"""
Morphological Analyzer Python Bindings
Modern implementation using JSON-based data loading
"""
import os
import json
from .loader import (
    load_bisaya_roots, 
    load_tagalog_roots
)

# Language Enum Mapping
LANG_MAP = {
    0: "Unknown",
    1: "Bisaya",
    2: "Tagalog",
    3: "Both"
}

class MorphologicalAnalyzer:
    """
    Pure Python morphological analyzer using JSON data files.
    No legacy C bindings - fully self-contained.
    """
    
    def __init__(self, project_root):
        self.project_root = project_root
        self.data_dir = os.path.join(project_root, "data")
        
        # Load all data files
        self._load_data()
        
    def _load_data(self):
        """Load all JSON data files"""
        # Load affix tables
        self.prefix_table = self._load_json("prefix_table.json")
        self.suffix_table = self._load_json("suffix_table.json")
        self.infix_table = self._load_json("infix_table.json")
        self.circumfix_table = self._load_json("circumfix_table.json")
        
        # Load root dictionaries (new format)
        self.bisaya_roots = load_bisaya_roots(
            os.path.join(self.data_dir, "bisaya_roots.json")
        )
        self.tagalog_roots = load_tagalog_roots(
            os.path.join(self.data_dir, "tagalog_roots.json")
        )
        
        # Build prefix/suffix lookup sets for fast matching
        self._build_affix_sets()
        
        print(f"Loaded {len(self.prefix_table)} prefixes")
        print(f"Loaded {len(self.suffix_table)} suffixes")
        print(f"Loaded {len(self.infix_table)} infixes")
        print(f"Loaded {len(self.circumfix_table)} circumfixes")
        print(f"Loaded {len(self.bisaya_roots)} Bisaya roots")
        print(f"Loaded {len(self.tagalog_roots)} Tagalog roots")
        
    def _load_json(self, filename):
        """Load a JSON file from data directory"""
        filepath = os.path.join(self.data_dir, filename)
        if os.path.exists(filepath):
            with open(filepath, 'r', encoding='utf-8') as f:
                return json.load(f)
        return {}
    
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
        Returns a list of ALL matching entries from both Bisaya and Tagalog dictionaries.
        """
        word_lower = word.lower()
        matches = []
        
        # Check Bisaya dictionary
        if word_lower in self.bisaya_roots:
            entry = self.bisaya_roots[word_lower]
            matches.append({
                **entry,
                "source": "bisaya_roots",
                "language": "Bisaya"
            })
        
        # Check Tagalog dictionary
        if word_lower in self.tagalog_roots:
            entry = self.tagalog_roots[word_lower]
            matches.append({
                **entry,
                "source": "tagalog_roots",
                "language": "Tagalog"
            })
        
        return matches if matches else None
    
    def lookup_root_first(self, word, language=None):
        """
        Look up a root word and return only the first match.
        Used for analysis where we need a single result.
        """
        matches = self.lookup_root(word, language)
        return matches[0] if matches else None
    
    def get_prefix_info(self, prefix_key):
        """Get full prefix information"""
        # Normalize key
        key = prefix_key if prefix_key.endswith('-') else prefix_key + '-'
        if key in self.prefix_table:
            return {
                "key": key,
                "definitions": self.prefix_table[key]
            }
        return None
    
    def get_suffix_info(self, suffix_key):
        """Get full suffix information"""
        key = suffix_key if suffix_key.startswith('-') else '-' + suffix_key
        if key in self.suffix_table:
            return {
                "key": key,
                "definitions": self.suffix_table[key]
            }
        return None

    def get_infix_info(self, infix_key):
        """Get full infix information"""
        key = infix_key if infix_key.startswith('-') else '-' + infix_key
        key = key if key.endswith('-') else key + '-'
        if key in self.infix_table:
            return {
                "key": key,
                "definitions": self.infix_table[key]
            }
        return None

    def get_circumfix_info(self, circumfix_key):
        """Get full circumfix information"""
        if circumfix_key in self.circumfix_table:
            return {
                "key": circumfix_key,
                "definitions": self.circumfix_table[circumfix_key]
            }
        return None
    
    def analyze(self, word):
        """Analyze a single word"""
        word_lower = word.lower()
        original = word
        
        result = {
            "word": original,
            "valid": False,
            "prefixes": [],
            "root": None,
            "suffixes": [],
            "infixes": [],
            "language": "Unknown",
            "definitions": [],
            "pos": None,
            "link": None,
            "structure": None,
            "affix_functions": [],
        }
        
        # Helper to populate result from matches
        def populate_from_matches(matches, root_word):
            result["valid"] = True
            result["root"] = root_word
            
            # Build definitions list from ALL matches
            for match in matches:
                result["definitions"].append({
                    "language": match.get("language", "Unknown"),
                    "definition": match.get("definition", ""),
                    "pos": match.get("pos"),
                    "link": match.get("link", ""),
                    "source": match.get("source", "")
                })
            
            # Set primary language based on matches
            if len(matches) == 1:
                result["language"] = self._normalize_language(matches[0].get("language", ""))
                result["pos"] = matches[0].get("pos")
                result["link"] = matches[0].get("link", "")
            elif len(matches) > 1:
                result["language"] = "Both"
                result["pos"] = matches[0].get("pos")
                result["link"] = matches[0].get("link", "")

        # 1. Try direct root lookup
        matches = self.lookup_root(word_lower)
        if matches:
            populate_from_matches(matches, word_lower)
            result["structure"] = f"[{word_lower}]"
            return result
            
        # 2. Try stripping Infixes
        # Simple Logic: Check for known infixes inside the word
        for infix in self.infix_forms:
            # Must be present and not at start/end (strictly inside)
            idx = word_lower.find(infix)
            if idx > 0 and idx < len(word_lower) - len(infix):
                # Only valid if surrounded by characters
                stripped = word_lower[:idx] + word_lower[idx+len(infix):]
                
                # Check if stripped version is a root
                matches = self.lookup_root(stripped)
                if matches:
                    result["infixes"] = [infix]
                    populate_from_matches(matches, stripped)
                    
                    # Get infix info
                    iinfo = self.get_infix_info(infix)
                    if iinfo and iinfo["definitions"]:
                        result["affix_functions"].append({
                            "affix": f"-{infix}-",
                            "type": "infix",
                            "function": iinfo["definitions"][0].get("function", "")
                        })
                    
                    # Structure: e.g. s[um]ulat
                    prefix_part = word_lower[:idx]
                    suffix_part = word_lower[idx+len(infix):]
                    result["structure"] = f"{prefix_part}[-{infix}-]{suffix_part}"
                    # Or standard: [-um-] + sulat
                    result["structure"] = f"[-{infix}-] + {stripped}" 
                    return result

        # 3. Try stripping Prefixes
        for prefix in self.prefix_forms:
            if word_lower.startswith(prefix) and len(word_lower) > len(prefix) + 2:
                remaining = word_lower[len(prefix):]
                matches = self.lookup_root(remaining)
                if matches:
                    result["prefixes"] = [prefix]
                    populate_from_matches(matches, remaining)
                    
                    pinfo = self.get_prefix_info(prefix)
                    if pinfo and pinfo["definitions"]:
                        result["affix_functions"].append({
                            "affix": prefix + "-",
                            "type": "prefix",
                            "function": pinfo["definitions"][0].get("function", "")
                        })
                    
                    result["structure"] = f"[{prefix}-] + {remaining}"
                    return result
                    
        # 4. Try stripping Suffixes
        for suffix in self.suffix_forms:
            if word_lower.endswith(suffix) and len(word_lower) > len(suffix) + 2:
                remaining = word_lower[:-len(suffix)]
                matches = self.lookup_root(remaining)
                if matches:
                    result["suffixes"] = [suffix]
                    populate_from_matches(matches, remaining)
                    
                    sinfo = self.get_suffix_info(suffix)
                    if sinfo and sinfo["definitions"]:
                        result["affix_functions"].append({
                            "affix": "-" + suffix,
                            "type": "suffix",
                            "function": sinfo["definitions"][0].get("function", "")
                        })
                    
                    result["structure"] = f"{remaining} + [-{suffix}]"
                    return result
        
        # 5. Try Prefix + Suffix (Check for Circumfix first)
        for prefix in self.prefix_forms:
            if word_lower.startswith(prefix):
                after_prefix = word_lower[len(prefix):]
                for suffix in self.suffix_forms:
                    if after_prefix.endswith(suffix) and len(after_prefix) > len(suffix):
                        # Ensure root has at least 2 chars
                        root_candidate = after_prefix[:-len(suffix)]
                        if len(root_candidate) < 2:
                            continue
                            
                        matches = self.lookup_root(root_candidate)
                        if matches:
                            # Check if this is a known Circumfix
                            circumfix_key = f"{prefix}-...-{suffix}"
                            cinfo = self.get_circumfix_info(circumfix_key)
                            
                            populate_from_matches(matches, root_candidate)
                            
                            if cinfo and cinfo["definitions"]:
                                # It's a circumfix
                                result["affix_functions"].append({
                                    "affix": circumfix_key,
                                    "type": "circumfix",
                                    "function": cinfo["definitions"][0].get("function", "")
                                })
                                result["structure"] = f"[{prefix}-] + {root_candidate} + [-{suffix}] (Circumfix)"
                            else:
                                # Treat as separate Prefix + Suffix
                                result["prefixes"] = [prefix]
                                result["suffixes"] = [suffix]
                                
                                pinfo = self.get_prefix_info(prefix)
                                if pinfo and pinfo["definitions"]:
                                    result["affix_functions"].append({
                                        "affix": prefix + "-",
                                        "type": "prefix",
                                        "function": pinfo["definitions"][0].get("function", "")
                                    })
                                
                                sinfo = self.get_suffix_info(suffix)
                                if sinfo and sinfo["definitions"]:
                                    result["affix_functions"].append({
                                        "affix": "-" + suffix,
                                        "type": "suffix",
                                        "function": sinfo["definitions"][0].get("function", "")
                                    })
                                
                                result["structure"] = f"[{prefix}-] + {root_candidate} + [-{suffix}]"
                            
                            return result
        
        # Not found - return as unknown
        result["structure"] = f"[{word_lower}]"
        return result
    
    def _normalize_language(self, lang):
        """Normalize language string"""
        lang_lower = lang.lower() if lang else ""
        if lang_lower in ("bisaya", "cebuano", "hiligaynon"):
            return "Bisaya"
        elif lang_lower == "tagalog":
            return "Tagalog"
        elif lang_lower in ("both", "shared", "austronesian", "spanish loan"):
            return "Both"
        return "Unknown"
    
    # API for browsing data
    
    def get_all_prefixes(self):
        """Get all prefix entries with their definitions"""
        return self.prefix_table
    
    def get_all_suffixes(self):
        """Get all suffix entries with their definitions"""
        return self.suffix_table
    
    def get_all_infixes(self):
        """Get all infix entries with their definitions"""
        return self.infix_table
    
    def get_all_circumfixes(self):
        """Get all circumfix entries with their definitions"""
        return self.circumfix_table
    
    def search_roots(self, query, language=None, limit=50):
        """Search roots by prefix"""
        query_lower = query.lower()
        results = []
        
        # Search Bisaya roots
        if language in (None, "Bisaya"):
            for word, info in self.bisaya_roots.items():
                if word.startswith(query_lower):
                    results.append({**info, "source": "Bisaya"})
                    if len(results) >= limit:
                        break
        
        # Search Tagalog roots
        if language in (None, "Tagalog") and len(results) < limit:
            for word, info in self.tagalog_roots.items():
                if word.startswith(query_lower):
                    results.append({**info, "source": "Tagalog"})
                    if len(results) >= limit:
                        break
        
        # Search shared vocab
        if language in (None, "Both") and len(results) < limit:
            for word, info in self.shared_vocab.items():
                if word.startswith(query_lower):
                    results.append({
                        "word": word,
                        "definition": info.get("meaning", ""),
                        "pos": info.get("pos"),
                        "language": info.get("origin", "Both"),
                        "source": "Shared"
                    })
                    if len(results) >= limit:
                        break
        
        return results
    
    def get_stats(self):
        """Get statistics about loaded data"""
        return {
            "prefixes": len(self.prefix_table),
            "suffixes": len(self.suffix_table),
            "infixes": len(self.infix_table),
            "circumfixes": len(self.circumfix_table),
            "bisaya_roots": len(self.bisaya_roots),
            "tagalog_roots": len(self.tagalog_roots),
            "total_roots": len(self.bisaya_roots) + len(self.tagalog_roots),
            "total_affixes": len(self.prefix_table) + len(self.suffix_table) + len(self.infix_table) + len(self.circumfix_table)
        }
