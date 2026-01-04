"""
Morphological Analyzer Python Bindings
Modern implementation using Finite Automaton and JSON-based data
"""
import os
import json
import collections
from .loader import (
    load_bisaya_roots, 
    load_tagalog_roots,
    load_tagalog_roots,
    load_affix_table
)
import logging

logger = logging.getLogger(__name__)

# Language Enum Mapping
LANG_MAP = {
    0: "Unknown",
    1: "Bisaya",
    2: "Tagalog",
    3: "Both"
}

class MorphologicalAutomaton:
    """
    Finite Automaton-based Morphological Analyzer.
    Uses BFS to explore all valid affix-stripping paths (Non-Deterministic).
    """
    def __init__(self, analyzer):
        self.analyzer = analyzer
        # Cache for performance
        self.memo = {}

    def analyze(self, word):
        """
        Run the automaton on a word.
        Returns a list of valid analyses (MorphologicalParse objects or dicts).
        """
        word_lower = word.lower()
        if not word_lower:
            return []

        # Queue item: (current_text, affixes_found_stack, state_metadata)
        # affixes_found_stack: list of { 'type': ..., 'text': ..., 'info': ... }
        # We strip from OUTSIDE IN.
        # Order of stripping: Suffixes, Prefixes, Infixes.
        # This acts as determining the path: Root -> (Infixed) -> (Prefixed) -> (Suffixed)
        # But computationally, we reverse it: Word -> Strip Suffix -> Strip Prefix -> Root.
        
        queue = collections.deque([ (word_lower, []) ])
        valid_results = []
        seen_states = set()

        processed_count = 0
        MAX_STEPS = 500 # Safety break for very complex words
        
        logger.debug(f"Analyzing word: '{word}'")

        while queue and processed_count < MAX_STEPS:
            processed_count += 1
            current_text, affixes = queue.popleft()
            
            # Log deep search
            # logger.debug(f"State: '{current_text}' | Affixes: {[a['text'] for a in affixes]}")
            
            state_key = (current_text, tuple(sorted([a['text'] for a in affixes])))
            if state_key in seen_states:
                continue
            seen_states.add(state_key)

            # 1. BASE CASE: CHECK ROOT (Transition to Accept State)
            # This handles "Root only" or "Stripped Affixes + Root"
            root_candidates = self._check_root_with_restoration(current_text, affixes)
            for root_match in root_candidates:
                valid_results.append(self._construct_result(word, root_match, affixes))
            
            # 2. TRANSITIONS (Strip Affixes)
            
            # A. Strip Suffixes (Valid only if we haven't stripped prefixes yet? 
            # Actually, standard order is usually flexible, but let's allow recursive stripping)
            # Strategy: Simply try to find any affix on the edge.
            
            # Try Suffixes
            for suffix in self.analyzer.suffix_forms:
                if current_text.endswith(suffix):
                    # Edge condition: must leave some root material
                    rem = current_text[:-len(suffix)]
                    if len(rem) >= 2: 
                        new_affixes = affixes + [{
                            'type': 'suffix', 
                            'text': suffix, 
                            'info': self.analyzer.get_suffix_info(suffix)
                        }]
                        queue.append((rem, new_affixes))

            # Try Prefixes
            for prefix in self.analyzer.prefix_forms:
                if current_text.startswith(prefix):
                    rem = current_text[len(prefix):]
                    if len(rem) >= 2:
                        new_affixes = affixes + [{
                            'type': 'prefix', 
                            'text': prefix, 
                            'info': self.analyzer.get_prefix_info(prefix)
                        }]
                        queue.append((rem, new_affixes))

            # Try Infixes (Can be anywhere)
            for infix in self.analyzer.infix_forms:
                # Optimized: Infixes usually appear near the start (e.g. s-um-ulat)
                # Scanning inside the whole word
                idx = current_text.find(infix)
                if idx > 0 and idx < len(current_text) - len(infix):
                     # Validate position (usually after first consonant)
                     # For now, allow loosely.
                     rem = current_text[:idx] + current_text[idx+len(infix):]
                     if len(rem) >= 2:
                        new_affixes = affixes + [{
                            'type': 'infix', 
                            'text': infix, 
                            'info': self.analyzer.get_infix_info(infix)
                        }]
                        queue.append((rem, new_affixes))
                        
        # 3. AMBIGUITY RESOLUTION & CLEANUP
        # Merge results that share the same Root + Affixes but have different definitions
        merged_results = {} # Key: (Root, AffixStr) -> ResultObj
        
        for res in valid_results:
            key = (res['root'], str(res['affix_functions']))
            
            if key not in merged_results:
                merged_results[key] = res
            else:
                # Merge definitions
                existing_defs = merged_results[key]['definitions']
                new_defs = res['definitions']
                
                # Add new defs if not present
                for nd in new_defs:
                    is_dupe = False
                    for ed in existing_defs:
                        if ed.get('definition') == nd.get('definition') and ed.get('language') == nd.get('language'):
                            is_dupe = True
                            break
                    if not is_dupe:
                        existing_defs.append(nd)
                        
                # Update merged result
                merged_results[key]['definitions'] = existing_defs
                
                # Check for "Both" language update
                if merged_results[key]['language'] != res['language']:
                     merged_results[key]['language'] = "Both"

        # Convert back to list
        unique_results = list(merged_results.values())
        
        # Sort by:
        # 1. Root Length (Longer is better, usually means less aggressive stemming)
        # 2. Number of Affixes (Fewer is better / Occam's Razor)
        # Sort by:
        # 1. Root Length (Longer is better, usually means less aggressive stemming)
        # 2. Number of Affixes (Fewer is better / Occam's Razor)
        # Primary: len(root) DESC
        # Secondary: Total affix count ASC (so negative len DESC)
         
        unique_results.sort(key=lambda x: (
            len(x['root']), 
            - (len(x['prefixes']) + len(x['suffixes']) + len(x['infixes']))
        ), reverse=True)
        # Actually:
        # Primary: len(root) DESC
        # Secondary: len(affixes) ASC
        # Python sort is stable. Let's use a tuple key.
        # We want MAX root length, then MIN affix count.
        # key = (len(root), -len(affixes)) -> (4, 0) > (4, -1) -> 4 > 3 is True. 0 > -1 is True.
        # Wait, (4, 0) > (4, -1).
        # result A (root): len 4, affixes 0 -> (4, 0)
        # result B (lupi): len 4, affixes 1 -> (4, -1)
        # We want A > B.
        # (4, 0) > (4, -1) is TRUE.
        # So sorting reverse=True with this key works.
         
        unique_results.sort(key=lambda x: (len(x['root']), -len(x['prefixes']) - len(x['suffixes']) - len(x['infixes'])), reverse=True)
        
        if unique_results:
            logger.info(f"Analysis for '{word}': Found {len(unique_results)} valid paths. Best: {unique_results[0]['root']}")
        else:
            logger.debug(f"No valid analysis found for '{word}'")
        
        # Post-process: Detect Circumfixes (Pairing Prefix+Suffix)
        for res in unique_results:
            self._detect_circumfixes(res)
            self._derive_pos(res)

        return unique_results

    def _check_root_with_restoration(self, text, current_affixes):
        """
        Check if text is a valid root, applying linguistic restoration rules.
        Returns list of root entries.
        """
        matches = []
        
        # 1. Direct Lookup
        direct = self.analyzer.lookup_root(text)
        if direct:
            matches.extend(direct)
            
        # 2. Restoration Logic (Allomorphs)
        # Look at the *last applied prefix* (which is the first in our stripped list?)
        # No, affixes list grows as we strip. 
        # If we stripped 'mam-', current_text is 'ato'.
        # We need to know WHICH prefix caused this.
        
        # Identify relevant prefix for assimilation
        relevant_prefix = None
        for a in current_affixes:
            if a['type'] == 'prefix':
                relevant_prefix = a['text']
                # The 'last' stripped prefix is loosely the outer one, 
                # but in assimilation (mang-bato -> mam-bato), the prefix is adjacent to the root.
                # Since we strip iteratively, we might have stripped 'mam-' directly.
                break
        
        if relevant_prefix:
            p = relevant_prefix
            # Nasal Assimilation Rules
            if p.endswith('m-') or p == 'mam-': # e.g. mam- (from mang-)
                # Restores P or B
                # Try adding P
                logger.debug(f"Restoring Assimilation (P/B) for prefix '{p}' on '{text}'")
                matches.extend(self.analyzer.lookup_root('p' + text) or [])
                # Try adding B
                matches.extend(self.analyzer.lookup_root('b' + text) or [])
                
            elif p.endswith('n-') or p == 'man-': # e.g. man-
                 # Restores T, D, S, L, R
                 logger.debug(f"Restoring Assimilation (T/D/S/L/R) for prefix '{p}' on '{text}'")
                 for char in ['t', 'd', 's', 'l', 'r']:
                     matches.extend(self.analyzer.lookup_root(char + text) or [])
                     
            elif p.endswith('ng-') or p == 'mang-': # e.g. mang-
                # Restores K, G, H, Vowel
                for char in ['k', 'g', 'h']:
                     matches.extend(self.analyzer.lookup_root(char + text) or [])
        
        # 3. Suffix Harmony / Vowel Shifts
        # e.g. kain + -in -> kanin (Syncope 'i' lost)
        # text = 'kan'
        # Need to restore vowel? Hard to guess.
        
        # e.g. kaon + -on -> kan-on (o -> u shift + syncope?)
        
        # Simple o -> u reverser
        if 'u' in text:
            # try swapping last u for o
            # very heuristic
            variants = [text.replace('u', 'o')] 
            for v in variants:
                matches.extend(self.analyzer.lookup_root(v) or [])
        
        # 4. Suffix Vowel Restoration (e.g. bilhin -> bil -> bili)
        # If the last stripped affix was a suffix like -hin/han/in/an
        # We try appending vowels to the remaining text
        relevant_suffix = None
        for a in current_affixes:
            if a['type'] == 'suffix':
                relevant_suffix = a['text']
                break
        
        if relevant_suffix:
            # Blindly try adding a vowel if root not found?
            if not matches and len(text) > 2:
                for v in ['i', 'a', 'o', 'u', 'e']:
                     matches.extend(self.analyzer.lookup_root(text + v) or [])

        # Dedupe matches
        unique_matches = []
        seen_entries = set()
        for m in matches:
            # Key now includes word AND language/definition to allow homonyms
            key = f"{m['word']}:{m.get('language')}:{m.get('definition')}"
            if key not in seen_entries:
                seen_entries.add(key)
                unique_matches.append(m)
                
        return unique_matches

    def _construct_result(self, original_word, root_entry, affixes):
        """Build the result object"""
        return {
            "word": original_word,
            "root": root_entry['word'],
            "valid": True,
            "language": root_entry.get('language', 'Unknown'),
            "definitions": [{
                "definition": root_entry.get('definition', ''),
                "pos": root_entry.get('pos', ''),
                "source": root_entry.get('source', ''),
                "language": root_entry.get('language', 'Unknown')  # FIX: Added language field
            }],
            "prefixes": [a['text'].rstrip('-') for a in affixes if a['type'] == 'prefix'],
            "suffixes": [a['text'].lstrip('-') for a in affixes if a['type'] == 'suffix'],
            "infixes": [a['text'].strip('-') for a in affixes if a['type'] == 'infix'],
            "affix_functions": [
                {
                    "affix": a['text'],
                    "type": a['type'],
                    "function": a['info']['definitions'][0].get('function', '') if a['info'] and a['info']['definitions'] else '',
                    "language": a['info']['definitions'][0].get('language', 'Unknown') if a['info'] and a['info']['definitions'] else 'Unknown',
                    "etymology": a['info']['definitions'][0].get('etymology', '') if a['info'] and a['info']['definitions'] else '',
                    "pronunciation": a['info']['definitions'][0].get('pronunciation', '') if a['info'] and a['info']['definitions'] else '',
                    "syllabification": a['info']['definitions'][0].get('syllabification', '') if a['info'] and a['info']['definitions'] else '',
                    "derived_terms": a['info']['definitions'][0].get('derived_terms', []) if a['info'] and a['info']['definitions'] else [],
                    "metadata": a['info']['definitions'][0] if a['info'] and a['info']['definitions'] else {}
                }
                for a in affixes
            ]
        }

    def _detect_circumfixes(self, result):
        """Identify circumfixes by pairing prefix+suffix"""
        # Simple heuristic: if we have prefix and suffix, check if they exist as a circumfix key
        p_list = result['prefixes']
        s_list = result['suffixes']
        
        if p_list and s_list:
            # Check combinations
            for p in p_list:
                for s in s_list:
                    key = f"{p}-...-{s}"
                    c_info = self.analyzer.get_circumfix_info(key)
                    if c_info:
                        # Mark as circumfix
                        result['affix_functions'].append({
                            "affix": key,
                            "type": "circumfix",
                            "function": c_info['definitions'][0].get('function', ''),
                            "metadata": c_info['definitions'][0]
                        })
                        
    def _derive_pos(self, result):
        """Derive final POS and metadata"""
        # Default to root POS
        final_pos = result['definitions'][0].get('pos', 'Unknown')
        grammar_notes = []
        
        # Iterate affixes (usually last one applied rules, but we just aggregate)
        for af in result['affix_functions']:
             meta = af.get('metadata', {})
             
             # Derived POS
             # If affix is Inflectional Verb Focus -> Verb
             if meta.get('focus'):
                 final_pos = "Verb" # Generic override
                 grammar_notes.append(f"Focus: {meta.get('focus')}")
             
             if meta.get('tense'):
                 grammar_notes.append(f"Tense: {meta.get('tense')}")
                 
             if meta.get('aspect'):
                 grammar_notes.append(f"Aspect: {meta.get('aspect')}")

        result['pos'] = final_pos
        result['grammar'] = ", ".join(grammar_notes)


class MorphologicalAnalyzer:
    """
    Main Logic Class.
    Loads data and delegates to Automaton.
    """
    
    def __init__(self, project_root):
        self.project_root = project_root
        self.data_dir = os.path.join(project_root, "data_v2")
        self.automaton = None # Late init
        
        # Load all data files
        self._load_data()
        
        # Init Automaton
        self.automaton = MorphologicalAutomaton(self)
        
    def _load_data(self):
        """Load all JSON data files"""
        # Load affix tables
        self.prefix_table = load_affix_table(os.path.join(self.data_dir, "prefix_table.json"))
        self.suffix_table = load_affix_table(os.path.join(self.data_dir, "suffix_table.json"))
        self.infix_table = load_affix_table(os.path.join(self.data_dir, "infix_table.json"))
        self.circumfix_table = load_affix_table(os.path.join(self.data_dir, "circumfix_table.json"))
        
        # Load root dictionaries
        self.bisaya_roots = load_bisaya_roots(os.path.join(self.data_dir, "bisaya_roots.json"))
        self.tagalog_roots = load_tagalog_roots(os.path.join(self.data_dir, "tagalog_roots.json"))
        
        # Build lookup sets
        self._build_affix_sets()
        
        print(f"Analyzer Loaded: {len(self.bisaya_roots)} Bisaya roots, {len(self.tagalog_roots)} Tagalog roots.")
    
    def _build_affix_sets(self):
        """Build sorted affix lists for matching (longest first)"""
        # Get prefix forms without trailing hyphen for matching
        self.prefix_forms = sorted(
            [k.rstrip('-') for k in self.prefix_table.keys()],
            key=len, reverse=True
        )
        # Get suffix forms without leading hyphen for matching
        self.suffix_forms = sorted(
            [k.lstrip('-') for k in self.suffix_table.keys()],
            key=len, reverse=True
        )
        # Infix forms
        self.infix_forms = sorted(
            [k.strip('-') for k in self.infix_table.keys()],
            key=len, reverse=True
        )
    
    def lookup_root(self, word, language=None):
        """
        Look up a root word in dictionaries.
        Returns a list of ALL matching entries.
        """
        word_lower = word.lower()
        matches = []
        
        # Check Bisaya dictionary
        if word_lower in self.bisaya_roots:
            entry = self.bisaya_roots[word_lower]
            matches.append({**entry, "source": "bisaya_roots", "language": "Bisaya"})
        
        # Check Tagalog dictionary
        if word_lower in self.tagalog_roots:
            entry = self.tagalog_roots[word_lower]
            matches.append({**entry, "source": "tagalog_roots", "language": "Tagalog"})
        
        return matches if matches else None
    
    # --- Affix Info Getters ---
    
    def get_prefix_info(self, prefix_key):
        key = prefix_key if prefix_key.endswith('-') else prefix_key + '-'
        if key in self.prefix_table:
            return {"key": key, "definitions": self.prefix_table[key]}
        return None
    
    def get_suffix_info(self, suffix_key):
        key = suffix_key if suffix_key.startswith('-') else '-' + suffix_key
        if key in self.suffix_table:
            return {"key": key, "definitions": self.suffix_table[key]}
        return None

    def get_infix_info(self, infix_key):
        key = infix_key if infix_key.startswith('-') else '-' + infix_key
        key = key if key.endswith('-') else key + '-'
        if key in self.infix_table:
            return {"key": key, "definitions": self.infix_table[key]}
        return None

    def get_circumfix_info(self, key):
        if key in self.circumfix_table:
            return {"key": key, "definitions": self.circumfix_table[key]}
        return None
    
    # --- Main Analysis ---

    def analyze(self, word):
        """Analyze a single word using the Automaton"""
        results = self.automaton.analyze(word)
        
        if results:
            # Return the best match (first one, which is longest root)
            return results[0]
            
        # Return empty/unknown structure
        return {
            "word": word,
            "valid": False,
            "root": None,
            "language": "Unknown",
            "structure": f"[{word}]",
            "affix_functions": [],
            "definitions": []
        }
    
    # --- Browsing API (Legacy Support) ---
    
    def get_all_prefixes(self): return self.prefix_table
    def get_all_suffixes(self): return self.suffix_table
    def get_all_infixes(self): return self.infix_table
    def get_all_circumfixes(self): return self.circumfix_table
    
    def get_stats(self):
        return {
            "prefixes": len(self.prefix_table),
            "suffixes": len(self.suffix_table),
            "infixes": len(self.infix_table),
            "circumfixes": len(self.circumfix_table),
            "bisaya_roots": len(self.bisaya_roots),
            "tagalog_roots": len(self.tagalog_roots)
        }
        
    def search_roots(self, query, language=None, limit=50):
        query_lower = query.lower()
        results = []
        if language in (None, "Bisaya"):
            for word, info in self.bisaya_roots.items():
                if word.startswith(query_lower):
                    results.append({**info, "source": "Bisaya"})
                    if len(results) >= limit: break
        if language in (None, "Tagalog") and len(results) < limit:
            for word, info in self.tagalog_roots.items():
                if word.startswith(query_lower):
                    results.append({**info, "source": "Tagalog"})
                    if len(results) >= limit: break
        return results


