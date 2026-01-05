"""
Morphological Analyzer - Trie-DFA Implementation
Pure functional approach with minimal abstraction
"""
import os
import json
import collections
from .utils import load_bisaya_roots, load_tagalog_roots, load_affix_table
import logging

logger = logging.getLogger(__name__)


# =============================================================================
# TRIE FUNCTIONS (build once, use for O(n) matching)
# =============================================================================

def build_trie(affixes, reverse=False):
    """Build a trie from affix list. Returns root node dict."""
    root = {}
    for affix in affixes:
        node = root
        chars = affix[::-1] if reverse else affix
        for char in chars:
            node = node.setdefault(char, {})
        node.setdefault('$', []).append(affix)  # '$' marks terminal
    return root


def find_prefixes(trie, text):
    """Find all prefixes matching start of text."""
    matches, node = [], trie
    for char in text:
        if char not in node:
            break
        node = node[char]
        matches.extend(node.get('$', []))
    return matches


def find_suffixes(trie, text):
    """Find all suffixes matching end of text (trie must be reversed)."""
    matches, node = [], trie
    for char in reversed(text):
        if char not in node:
            break
        node = node[char]
        matches.extend(node.get('$', []))
    return matches


def find_infixes(trie, text):
    """Find infixes anywhere in text (not at edges)."""
    matches = []
    for start in range(1, len(text) - 1):
        node = trie
        for i in range(start, len(text) - 1):
            if text[i] not in node:
                break
            node = node[text[i]]
            for key in node.get('$', []):
                if start + len(key) < len(text):
                    matches.append((start, key))
    return matches


# =============================================================================
# CORE ANALYZER
# =============================================================================

class MorphologicalAnalyzer:
    """
    Main analyzer. Uses tries for fast affix matching.
    Class exists to hold loaded data and provide clean API.
    """
    
    def __init__(self, project_root):
        data_dir = os.path.join(project_root, "data_v2")
        
        # Load data
        self.prefix_table = load_affix_table(os.path.join(data_dir, "prefix_table.json"))
        self.suffix_table = load_affix_table(os.path.join(data_dir, "suffix_table.json"))
        self.infix_table = load_affix_table(os.path.join(data_dir, "infix_table.json"))
        self.circumfix_table = load_affix_table(os.path.join(data_dir, "circumfix_table.json"))
        self.bisaya_roots = load_bisaya_roots(os.path.join(data_dir, "bisaya_roots.json"))
        self.tagalog_roots = load_tagalog_roots(os.path.join(data_dir, "tagalog_roots.json"))
        
        # Build tries
        self.prefix_trie = build_trie([k.rstrip('-') for k in self.prefix_table])
        self.suffix_trie = build_trie([k.lstrip('-') for k in self.suffix_table], reverse=True)
        self.infix_trie = build_trie([k.strip('-') for k in self.infix_table])
        
        print(f"Loaded: {len(self.bisaya_roots)} Bisaya, {len(self.tagalog_roots)} Tagalog roots")

    def analyze(self, word):
        """Analyze a word using BFS with trie-based affix matching."""
        word_lower = word.lower()
        if not word_lower:
            return {"word": word, "valid": False, "root": None, "language": "Unknown", 
                    "affix_functions": [], "definitions": []}

        queue = collections.deque([(word_lower, [])])
        results, seen = [], set()

        while queue and len(seen) < 500:
            current, affixes = queue.popleft()
            key = (current, tuple(sorted(a[1] for a in affixes)))
            if key in seen:
                continue
            seen.add(key)

            # Check if current is a valid root
            for root in self._lookup_with_restore(current, affixes):
                results.append(self._format_result(word, root, affixes))

            if len(current) <= 2:
                continue

            # Try stripping affixes using trie matching
            for suffix in find_suffixes(self.suffix_trie, current):
                rem = current[:-len(suffix)]
                if len(rem) >= 2:
                    queue.append((rem, affixes + [('suffix', suffix)]))

            for prefix in find_prefixes(self.prefix_trie, current):
                rem = current[len(prefix):]
                if len(rem) >= 2:
                    queue.append((rem, affixes + [('prefix', prefix)]))

            for start, infix in find_infixes(self.infix_trie, current):
                rem = current[:start] + current[start + len(infix):]
                if len(rem) >= 2:
                    queue.append((rem, affixes + [('infix', infix)]))

        # Merge duplicates and sort (longer root, fewer affixes = better)
        merged = {}
        for r in results:
            k = (r['root'], tuple(r['prefixes']), tuple(r['suffixes']), tuple(r['infixes']))
            if k not in merged:
                merged[k] = r
            elif merged[k]['language'] != r['language']:
                merged[k]['language'] = "Both"
                merged[k]['definitions'].extend(r['definitions'])

        final = sorted(merged.values(), key=lambda x: (len(x['root']), -len(x['prefixes']) - len(x['suffixes']) - len(x['infixes'])), reverse=True)
        return final[0] if final else {"word": word, "valid": False, "root": None, "language": "Unknown", "affix_functions": [], "definitions": []}

    def _lookup_with_restore(self, text, affixes):
        """Lookup root with morphophonemic restoration."""
        matches = []
        
        # Direct lookup
        if text in self.bisaya_roots:
            matches.append({**self.bisaya_roots[text], "source": "bisaya", "language": "Bisaya"})
        if text in self.tagalog_roots:
            matches.append({**self.tagalog_roots[text], "source": "tagalog", "language": "Tagalog"})

        # Nasal assimilation restoration
        prefix = next((a[1] for a in affixes if a[0] == 'prefix'), None)
        if prefix:
            restore = {'m': 'pb', 'n': 'tdslr', 'ng': 'kgh', 'mam': 'pb', 'man': 'tdslr', 'mang': 'kgh'}
            for p, chars in restore.items():
                if prefix.endswith(p) or prefix == p:
                    for c in chars:
                        restored = c + text
                        if restored in self.bisaya_roots:
                            matches.append({**self.bisaya_roots[restored], "source": "bisaya", "language": "Bisaya"})
                        if restored in self.tagalog_roots:
                            matches.append({**self.tagalog_roots[restored], "source": "tagalog", "language": "Tagalog"})
                    break

        # Vowel harmony (u -> o)
        if 'u' in text:
            alt = text.replace('u', 'o')
            if alt in self.bisaya_roots:
                matches.append({**self.bisaya_roots[alt], "source": "bisaya", "language": "Bisaya"})
            if alt in self.tagalog_roots:
                matches.append({**self.tagalog_roots[alt], "source": "tagalog", "language": "Tagalog"})

        # Dedupe
        seen = set()
        return [m for m in matches if not (k := f"{m['word']}:{m['language']}") in seen and not seen.add(k)]

    def _format_result(self, word, root, affixes):
        """Format analysis result."""
        def get_info(atype, text):
            table = {'prefix': self.prefix_table, 'suffix': self.suffix_table, 'infix': self.infix_table}[atype]
            fmt = {'prefix': f'{text}-', 'suffix': f'-{text}', 'infix': f'-{text}-'}[atype]
            defs = table.get(fmt, [])
            d = defs[0] if defs else {}
            return {"affix": text, "type": atype, "function": d.get('function', ''), 
                    "language": d.get('language', 'Unknown'), "metadata": d}

        return {
            "word": word, "root": root['word'], "valid": True,
            "language": root.get('language', 'Unknown'),
            "definitions": [{"definition": root.get('definition', ''), "pos": root.get('pos', ''),
                           "source": root.get('source', ''), "language": root.get('language', 'Unknown')}],
            "prefixes": [a[1] for a in affixes if a[0] == 'prefix'],
            "suffixes": [a[1] for a in affixes if a[0] == 'suffix'],
            "infixes": [a[1] for a in affixes if a[0] == 'infix'],
            "affix_functions": [get_info(a[0], a[1]) for a in affixes]
        }

    def lookup_root(self, word):
        """Public API for root lookup."""
        w = word.lower()
        matches = []
        if w in self.bisaya_roots:
            matches.append({**self.bisaya_roots[w], "source": "bisaya", "language": "Bisaya"})
        if w in self.tagalog_roots:
            matches.append({**self.tagalog_roots[w], "source": "tagalog", "language": "Tagalog"})
        return matches or None

    # --- API for web server ---
    def get_all_prefixes(self): return self.prefix_table
    def get_all_suffixes(self): return self.suffix_table
    def get_all_infixes(self): return self.infix_table
    def get_all_circumfixes(self): return self.circumfix_table
    
    def get_stats(self):
        return {"prefixes": len(self.prefix_table), "suffixes": len(self.suffix_table),
                "infixes": len(self.infix_table), "circumfixes": len(self.circumfix_table),
                "bisaya_roots": len(self.bisaya_roots), "tagalog_roots": len(self.tagalog_roots)}

    def search_roots(self, query, language=None, limit=50):
        q = query.lower()
        results = []
        if language in (None, "Bisaya"):
            for word, info in self.bisaya_roots.items():
                if word.startswith(q):
                    results.append({**info, "source": "Bisaya"})
                    if len(results) >= limit: break
        if language in (None, "Tagalog") and len(results) < limit:
            for word, info in self.tagalog_roots.items():
                if word.startswith(q):
                    results.append({**info, "source": "Tagalog"})
                    if len(results) >= limit: break
        return results
