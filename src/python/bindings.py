"""
Morphological Analyzer - Formal Morphotactic FSM Implementation

FORMAL DEFINITION:
Morphotactics are modeled by a finite-state recognizer M = (Q, Σ, δ, q₀, F)
where:
  Q = States encoding morphological categories (prefix, infix, root, suffix/circumfix)
  Σ = Surface alphabet of the language {a-z, -, '}
  δ = Transition function encoding legal affix sequences
  q₀ = Initial morphotactic state
  F = Final (accepting) morphological configurations

ARCHITECTURE:
Morphology is divided into four interacting components controlled by a global automaton:
1. PrefixFSM: Lexical automaton for prefix tokens
2. InfixFSM: ε-transitions inserting morphemes
3. RootLexicon: Stem lexicon lookup structure (validates stems AFTER shape is licensed)
4. SuffixCircumfixFSM: Lexical automaton for suffixes + circumfix constraints

The global control automaton licenses structures of the form:
(PREFIX)* (INFIX)? ROOT (SUFFIX)* (CIRCUMFIX constraint)*
"""
import os
import collections
from .utils import load_bisaya_roots, load_tagalog_roots, load_affix_table
import logging

logging.basicConfig(level=logging.INFO, format='%(message)s')
logger = logging.getLogger(__name__)

EPSILON = 'ε'

# Morphotactic Regions
REGION_PREFIX = 0
REGION_ROOT = 1
REGION_SUFFIX = 2


class LexicalAutomaton:
    """Base class for lexical resources (Tries/FSMs)."""
    def __init__(self, words):
        self.trie, self.accept = self._build_trie(words)

    def _build_trie(self, words):
        trie = [{}]
        accept = set()
        for word in words:
            state = 0
            for char in word:
                if char not in trie[state]:
                    trie.append({})
                    trie[state][char] = len(trie) - 1
                state = trie[state][char]
            accept.add(state)
        return trie, accept

    def get_transitions(self, state, char):
        if state < len(self.trie) and char in self.trie[state]:
            return self.trie[state][char]
        return None

    def get_info(self, key):
        return [] # Override


class PrefixFSM(LexicalAutomaton):
    """
    Lexical automaton para sa mga prefix tokens.
    (Lexical automaton for prefix tokens.)
    """
    def __init__(self, prefix_table):
        self.table = prefix_table
        # Tanggalin ang hyphen sa dulo (e.g., 'mag-' -> 'mag') para sa trie lookup
        prefixes = [k.rstrip('-') for k in prefix_table.keys()]
        super().__init__(prefixes)

    def get_info(self, prefix):
        # Ibalik ang full metadata mula sa JSON (kasama definition, function, etc.)
        return self.table.get(prefix + '-', [])


class SuffixCircumfixFSM(LexicalAutomaton):
    """
    Lexical automaton para sa suffix tokens at circumfix constraints.
    Gamit ang forward trie para sa Left-to-Right parsing.
    """
    def __init__(self, suffix_table, circumfix_table):
        self.suffix_table = suffix_table
        self.circumfix_table = circumfix_table
        
        # Kunin ang suffix keys at alisin ang hyphen sa unahan (e.g., '-an' -> 'an')
        suffixes = [k.lstrip('-') for k in suffix_table.keys()]
        
        # Idagdag din ang suffixes na bahagi ng circumfix (e.g., 'an' from 'ka-...-an')
        self.circumfix_suffixes = {} 
        for cf in circumfix_table:
            parts = cf.split('-...-')
            if len(parts) == 2:
                s = parts[1]
                suffixes.append(s)

        super().__init__(suffixes)


class RootLexicon:
    """
    Stem lexicon lookup structure.
    Tinitiyak nito na valid ang root word matapos ma-identify ang shape ng FSM.
    """
    def __init__(self, bisaya, tagalog):
        self.bisaya = bisaya
        self.tagalog = tagalog
        all_roots = set(bisaya.keys()) | set(tagalog.keys())
        
        self.trie = [{}]
        self.accept = set()
        # Mapping mula FSM state papunta sa listahan ng actual valid roots
        self.state_to_roots = {} 
        
        for root in all_roots:
            state = 0
            for char in root:
                if char not in self.trie[state]:
                    self.trie.append({})
                    self.trie[state][char] = len(self.trie) - 1
                state = self.trie[state][char]
            self.accept.add(state)
            if state not in self.state_to_roots:
                self.state_to_roots[state] = []
            self.state_to_roots[state].append(root)

    def get_transitions(self, state, char):
        if state < len(self.trie) and char in self.trie[state]:
            return self.trie[state][char]
        return None

    def get_info(self, root):
        info = []
        if root in self.bisaya:
            info.append({**self.bisaya[root], 'language': 'Bisaya'})
        if root in self.tagalog:
            info.append({**self.tagalog[root], 'language': 'Tagalog'})
        return info


class InfixFSM:
    """
    Handles infix patterns using ε-transition logic.
    Ang mga infixes ay 'sinisingit' sa loob ng root kaya espesyal ang handling nito.
    """
    def __init__(self, infix_table):
        self.infixes = [k.strip('-') for k in infix_table.keys()]
        self.table = infix_table

    def check_infix(self, word_slice):
        """
        Check kung ang simula ng word_slice ay valid na infix.
        (Check if start of word_slice matches an infix.)
        """
        matches = []
        for infix in self.infixes:
            if word_slice.startswith(infix):
                matches.append(infix)
        return matches

    def get_info(self, infix):
         return self.table.get('-' + infix + '-', [])


class MorphotacticFSM:
    """
    Global control automaton.
    Ito ang 'utak' na nagko-coordinate sa 4 na components: Prefix, Infix, Root, at Suffix.
    Determines valid sequences of affixes and roots.
    """
    def __init__(self, prefix_fsm, infix_fsm, root_lexicon, suffix_fsm):
        self.prefix_fsm = prefix_fsm
        self.infix_fsm = infix_fsm
        self.root_lexicon = root_lexicon
        self.suffix_fsm = suffix_fsm

    def simulate(self, word, logger=None):
        """
        Nondeterministic simulation ng morphotactic FSM.
        Sinusubukan lahat ng posibleng path para mahanap ang valid analysis.
        
        Returns all valid analyses found.
        """
        word = word.lower()
        if logger: logger.info(f"Starting Simulation: '{word}'")
        
        # Configuration: (region, state, pos, buffer, data)
        # buffer: characters na na-scan pero di pa "nako-commit" (hinahawakan muna)
        initial = {
            'region': REGION_PREFIX, 'state': 0, 'pos': 0, 
            'buffer': '', 
            'data': {'prefixes': [], 'infixes': [], 'root': '', 'suffixes': []}
        }
        
        queue = collections.deque([initial])
        # State signature para sa visited check (iwas infinite loop)
        seen = set()
        results = []
        step = 0
        
        region_names = {0: 'PREFIX', 1: 'ROOT', 2: 'SUFFIX'}
        
        while queue and step < 50000:
            config = queue.popleft()
            region = config['region']
            state = config['state']
            pos = config['pos']
            buf = config['buffer']
            data = config['data']
            
            state_desc = f"[R:{region_names[region]} S:{state} Pos:{pos} Buf:'{buf}']"
            # if logger: logger.info(f"Pop: {state_desc}")

            # Pruning / Visited Check
            sig = (region, state, pos, tuple(data['prefixes']), tuple(data['suffixes']))
            if sig in seen: continue
            seen.add(sig)
            step += 1
            
            # ---------------------------------------------------------
            # END OF INPUT CHECK (Tapos na ba ang input string?)
            # ---------------------------------------------------------
            if pos == len(word):
                # 1. Tumigil sa ROOT accept state?
                if region == REGION_ROOT and state in self.root_lexicon.accept:
                    # Valid root end. Verify natin kung valid root talaga yung na-buffer.
                    if buf in self.root_lexicon.state_to_roots.get(state, []):
                        if logger: logger.info(f"  -> ACCEPT (ROOT End): {state_desc} Root='{buf}'")
                        res = data.copy()
                        res['root'] = buf
                        results.append(res)
                
                # 2. Tumigil sa SUFFIX accept state?
                elif region == REGION_SUFFIX and state in self.suffix_fsm.accept:
                    if logger: logger.info(f"  -> ACCEPT (SUFFIX End): {state_desc} Suffix='{buf}'")
                    res = data.copy()
                    res['suffixes'] = res['suffixes'] + [buf]
                    results.append(res)

            # ---------------------------------------------------------
            # TRANSITIONS (δ)
            # ---------------------------------------------------------
            char = word[pos] if pos < len(word) else None
            
            # 1. CHARACTER MATCH (Galaw sa loob ng current region)
            if char:
                next_s = None
                if region == REGION_PREFIX:
                    next_s = self.prefix_fsm.get_transitions(state, char)
                elif region == REGION_ROOT:
                    next_s = self.root_lexicon.get_transitions(state, char)
                elif region == REGION_SUFFIX:
                    next_s = self.suffix_fsm.get_transitions(state, char)
                    
                if next_s is not None:
                    # if logger: logger.info(f"  -> TRANSITION (Char '{char}'): {state_desc} -> S:{next_s}")
                    queue.append({
                        'region': region, 'state': next_s, 'pos': pos+1,
                        'buffer': buf + char, 'data': data
                    })

            # 2. EPSILON TRANSITIONS (Lipat ng Region / Phase Change)
            
            # PREFIX -> ROOT (Start Root Processing)
            # Pwede lang lumipat kung nasa "start" or "valid end" ng prefix trie.
            if region == REGION_PREFIX:
                 if state == 0 or state in self.prefix_fsm.accept:
                     # Commit prefix token
                     new_data = data.copy()
                     if buf: 
                         if logger: logger.info(f"  -> EPSILON (Commit PREFIX): '{buf}'")
                         new_data['prefixes'] = new_data['prefixes'] + [buf]
                     
                     # 2a. Direct Transition to ROOT (ε)
                     if logger: logger.info(f"  -> EPSILON (PREFIX->ROOT): {state_desc} -> Root Start")
                     queue.append({
                         'region': REGION_ROOT, 'state': 0, 'pos': pos,
                         'buffer': '', 'data': new_data
                     })
                     
                     # 2b. INFIX Transition (Standard Tagalog -um-/-in- after 1st char)
                     # Pattern: C + Infix + RootRest
                     remaining = word[pos:]
                     if len(remaining) > 0:
                         first_char = remaining[0]
                         rest = remaining[1:]
                         
                         # Check possible infixes in 'rest'
                         infix_matches = self.infix_fsm.check_infix(rest)
                         for infix in infix_matches:
                             # Found infix! e.g. "in" in "s-in-ulat"
                             if logger: logger.info(f"  -> INFIX MATCH (Inner): '{infix}' inside '{remaining}'")
                             
                             root_next_s = self.root_lexicon.get_transitions(0, first_char)
                             if root_next_s is not None:
                                 inf_data = new_data.copy()
                                 inf_data['infixes'] = inf_data['infixes'] + [infix]
                                 
                                 queue.append({
                                     'region': REGION_ROOT, 
                                     'state': root_next_s, 
                                     'pos': pos + 1 + len(infix),
                                     'buffer': first_char, # We consumed 's'
                                     'data': inf_data
                                 })

                     # 2c. Infix at start (vowel roots e.g., in-abot)
                     infix_matches = self.infix_fsm.check_infix(remaining)
                     for infix in infix_matches:
                         if logger: logger.info(f"  -> INFIX MATCH (Start): '{infix}' at '{remaining}'")
                         inf_data = new_data.copy()
                         inf_data['infixes'] = inf_data['infixes'] + [infix]
                         queue.append({
                             'region': REGION_ROOT, 'state': 0, 'pos': pos + len(infix),
                             'buffer': '', 'data': inf_data
                         })

            # ROOT -> SUFFIX
            # Condition: Dapat nasa valid root accept state tayo.
            if region == REGION_ROOT:
                if state in self.root_lexicon.accept:
                     # Verify root validity
                     if buf in self.root_lexicon.state_to_roots.get(state, []):
                         if logger: logger.info(f"  -> EPSILON (ROOT->SUFFIX): '{buf}' verified.")
                         new_data = data.copy()
                         new_data['root'] = buf
                         
                         # Transition to SUFFIX FSM start (state 0)
                         queue.append({
                             'region': REGION_SUFFIX, 'state': 0, 'pos': pos,
                             'buffer': '', 'data': new_data
                         })
            
            # SUFFIX -> SUFFIX (Chained suffixes / Recursive)
            if region == REGION_SUFFIX:
                 if state in self.suffix_fsm.accept:
                     # Commit current suffix
                     if logger: logger.info(f"  -> EPSILON (Chain SUFFIX): '{buf}'")
                     new_data = data.copy()
                     new_data['suffixes'] = new_data['suffixes'] + [buf]
                     
                     queue.append({
                         'region': REGION_SUFFIX, 'state': 0, 'pos': pos,
                         'buffer': '', 'data': new_data
                     })

        return results


class MorphologicalAnalyzer:
    """Main Interface."""
    def __init__(self, project_root):
        data_dir = os.path.join(project_root, "data_v2")
        
        self.prefix_table = load_affix_table(os.path.join(data_dir, "prefix_table.json"))
        self.suffix_table = load_affix_table(os.path.join(data_dir, "suffix_table.json"))
        self.infix_table = load_affix_table(os.path.join(data_dir, "infix_table.json"))
        self.circumfix_table = load_affix_table(os.path.join(data_dir, "circumfix_table.json"))
        self.bisaya_roots = load_bisaya_roots(os.path.join(data_dir, "bisaya_roots.json"))
        self.tagalog_roots = load_tagalog_roots(os.path.join(data_dir, "tagalog_roots.json"))
        
        self.prefix_fsm = PrefixFSM(self.prefix_table)
        self.infix_fsm = InfixFSM(self.infix_table)
        self.root_lexicon = RootLexicon(self.bisaya_roots, self.tagalog_roots)
        self.suffix_fsm = SuffixCircumfixFSM(self.suffix_table, self.circumfix_table)
        
        self.fsm = MorphotacticFSM(
            self.prefix_fsm, self.infix_fsm, self.root_lexicon, self.suffix_fsm
        )
        print(f"Loaded: {len(self.bisaya_roots)} Bisaya, {len(self.tagalog_roots)} Tagalog roots")



    def _generate_morphotactic_parse(self, res):
        """
        Generates the 'Finite-State Morphotactic Parse' output.
        Format: morpheme[TAG] + ...
        """
        parts = []
        
        # Prefixes
        # Check for circumfix prefixes
        # Note: Ideally we'd link strict circumfix parsing, but for display we use the type
        # If we have logic to detect circumfixes explicitly in the result, we use it.
        # For now, we follow the standard tagging.
        
        for p in res['prefixes']:
            parts.append(f"{p}[PFX]")
            
        # Infixes
        # Standard display puts infixes before root or inside. 
        # For linear representation: Infix[INFX] + Root[ROOT] is acceptable common notation
        # unless we want to show the split root. 
        # User example: in[INFX] + sulat[ROOT]
        for i in res['infixes']:
            parts.append(f"{i}[INFX]")
            
        # Root
        parts.append(f"{res['root']}[ROOT]")
        
        # Suffixes
        for s in res['suffixes']:
            parts.append(f"{s}[SFX]")
        
        return " + ".join(parts)

    def _empty_result(self, word):
        return {
            "word": word, "valid": False, "root": None, "language": "Unknown",
            "affix_functions": [], "definitions": [], "prefixes": [], "suffixes": [], "infixes": [],
            "formatted_analysis": ""
        }
        
    def analyze(self, word):
        # Configure file logging for comprehensive FSM Trace
        fsm_logger = logging.getLogger('fsm_trace')
        handler = logging.FileHandler('fsm_trace.log', mode='w', encoding='utf-8')
        formatter = logging.Formatter('%(message)s')
        handler.setFormatter(formatter)
        fsm_logger.addHandler(handler)
        fsm_logger.setLevel(logging.INFO)
        
        fsm_logger.info(f"=== FSM TRACE START: {word} ===")
        fsm_logger.info(f"Input: {word}")
        
        try:
            raw_results = self.fsm.simulate(word, fsm_logger)
            fsm_logger.info(f"=== FSM TRACE END: Found {len(raw_results)} raw candidates ===")
            
            if not raw_results:
                 res = self._empty_result(word)
            else:
                 res = self._format_results(word, raw_results, fsm_logger)
        finally:
            fsm_logger.removeHandler(handler)
            handler.close()
            
        return res

    def _format_results(self, word, raw_results, logger=None):
        formatted = []
        seen = set()
        
        for res in raw_results:
            root = res['root']
            # Deduplicate by signature (root + affixes)
            key = (root, tuple(res['prefixes']), tuple(res['suffixes']), tuple(res['infixes']))
            if key in seen: continue
            seen.add(key)
            
            infos = self.root_lexicon.get_info(root)
            if not infos: continue
            
            # Affix details (Include full metadata from loaded tables)
            affixes = []
            for p in res['prefixes']:
                info_list = self.prefix_fsm.get_info(p)
                # Kunin ang unang entry bilang primary metadata
                meta = info_list[0] if info_list else {}
                entry = {'type': 'prefix', 'affix': p}
                entry.update(meta)
                affixes.append(entry)
                
            for i in res['infixes']:
                info_list = self.infix_fsm.get_info(i)
                meta = info_list[0] if info_list else {}
                entry = {'type': 'infix', 'affix': i}
                entry.update(meta)
                affixes.append(entry)
                
            for s in res['suffixes']:
                # Lookup full suffix info
                info_list = self.suffix_table.get('-' + s, [])
                meta = info_list[0] if info_list else {}
                entry = {'type': 'suffix', 'affix': s}
                entry.update(meta)
                affixes.append(entry)

            parse_string = self._generate_morphotactic_parse(res)

            entry = {
                "word": word, "root": root,
                "language": infos[0]['language'],
                "definitions": [],
                "prefixes": res['prefixes'],
                "suffixes": res['suffixes'],
                "infixes": res['infixes'],
                "affix_functions": affixes, # Now fully populated
                "formatted_analysis": parse_string,
                "valid": True
            }
            
            langs = set()
            for info in infos:
                entry['definitions'].append({
                    "definition": info.get('definition', ''),
                    "language": info['language'],
                    "pos": info.get('pos', '')
                })
                langs.add(info['language'])
            if len(langs) > 1: entry['language'] = "Both"
            
            formatted.append(entry)

        if not formatted: return self._empty_result(word)
        
        # Sort by root length (heuristic: mas mahabang root, mas likely tama)
        formatted.sort(key=lambda x: len(x['root']), reverse=True)
        
        if logger:
            logger.info(f"=== RANKED CANDIDATES ({len(formatted)}) ===")
            for i, cand in enumerate(formatted):
                logger.info(f"{i+1}. {cand['formatted_analysis']} (Root: {cand['root']})")
                
        return formatted[0]

    # API Compliance Methods
    def lookup_root(self, word): return self.root_lexicon.get_info(word) or None
    
    def get_stats(self): 
        return {
            "prefixes": len(self.prefix_table),
            "suffixes": len(self.suffix_table),
            "infixes": len(self.infix_table),
            "circumfixes": len(self.circumfix_table),
            "bisaya_roots": len(self.bisaya_roots),
            "tagalog_roots": len(self.tagalog_roots)
        }

    def search_roots(self, q, language=None, limit=50):
        q = q.lower()
        results = []
        
        # Search Bisaya
        if language in (None, "Bisaya"):
            for root, data in self.bisaya_roots.items():
                if q in root:
                    results.append({**data, "source": "Bisaya", "root": root})
                    if len(results) >= limit: break
        
        # Search Tagalog
        if len(results) < limit and language in (None, "Tagalog"):
            for root, data in self.tagalog_roots.items():
                if q in root:
                    results.append({**data, "source": "Tagalog", "root": root})
                    if len(results) >= limit: break
                    
        return results

    def get_all_prefixes(self): return self.prefix_table
    def get_all_suffixes(self): return self.suffix_table
    def get_all_infixes(self): return self.infix_table
    def get_all_circumfixes(self): return self.circumfix_table
    
    def get_prefix_info(self, key):
        return self.prefix_fsm.get_info(key)
        
    def get_suffix_info(self, key):
        # Suffix table keys usually have hyphen like '-an'
        # The key passed might need a hyphen added if not present
        if not key.startswith('-'):
            key = '-' + key
        return self.suffix_table.get(key, [])
