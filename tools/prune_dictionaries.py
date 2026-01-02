import json
import os
import re

DATA_DIR = "data"
AFFIX_FILE = "unified_bistag_affix.json"
ROOT_FILES = [
    "cebuano_roots_clean.json",
    "tagalog_roots_clean.json",
    "hiligaynon_roots_clean.json"
]

def load_json(filename):
    path = os.path.join(DATA_DIR, filename)
    if not os.path.exists(path):
        print(f"File not found: {path} - Skipping")
        return []
        
    with open(path, 'r', encoding='utf-8') as f:
        return json.load(f)

def save_json(filename, data):
    path = os.path.join(DATA_DIR, filename)
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    print(f"Saved: {path}")

def extract_affixes(node, affixes):
    if isinstance(node, dict):
        if "affixes" in node and isinstance(node["affixes"], list):
            for afx in node["affixes"]:
                if isinstance(afx, str):
                    affixes.add(afx)
                elif isinstance(afx, dict) and "affix" in afx:
                    affixes.add(afx["affix"])
        
        # Also check separate "affix" key if it exists outside
        if "affix" in node and isinstance(node["affix"], str):
             affixes.add(node["affix"])

        for key, value in node.items():
            extract_affixes(value, affixes)
    elif isinstance(node, list):
        for item in node:
            extract_affixes(item, affixes)

def categorize_affixes(affix_set):
    prefixes = []
    suffixes = []
    infixes = []
    circumfixes = []
    
    for afx in affix_set:
        if "..." in afx:
            # circumfix: e.g. "nag-...-an"
            parts = afx.split("...")
            if len(parts) == 2:
                # remove hyphens from inner sides? "nag-" "an"
                p = parts[0].replace("-", "")
                s = parts[1].replace("-", "")
                circumfixes.append((p, s))
        elif afx.startswith("-") and afx.endswith("-"):
            # infix: "-um-"
            infixes.append(afx.replace("-", ""))
        elif afx.endswith("-"):
            # prefix: "nag-"
            prefixes.append(afx.replace("-", ""))
        elif afx.startswith("-"):
            # suffix: "-an"
            suffixes.append(afx.replace("-", ""))
            
    # Sort by length descending (greedy matching)
    prefixes.sort(key=len, reverse=True)
    suffixes.sort(key=len, reverse=True)
    infixes.sort(key=len, reverse=True)
    # circumfixes ??? Tuple sort?
    circumfixes.sort(key=lambda x: len(x[0]) + len(x[1]), reverse=True)
    
    return prefixes, suffixes, infixes, circumfixes

def is_affixed(word, root_set, prefixes, suffixes, infixes, circumfixes):
    # Check Circumfixes first
    for p, s in circumfixes:
        if word.startswith(p) and word.endswith(s):
            root_cand = word[len(p):-len(s)]
            if len(root_cand) > 1 and root_cand in root_set:
                return True, f"circumfix({p}...{s})", root_cand

    # Check Prefixes
    for p in prefixes:
        if word.startswith(p):
            root_cand = word[len(p):]
            if len(root_cand) > 1 and root_cand in root_set:
                return True, f"prefix({p})", root_cand
                
    # Check Suffixes
    for s in suffixes:
        if word.endswith(s):
            root_cand = word[:-len(s)]
            if len(root_cand) > 1 and root_cand in root_set:
                return True, f"suffix({s})", root_cand
                
    # Check Infixes
    # Simple heuristic: infix inserted after first char, or first consonant cluster?
    # e.g. "kumain" -> "kain". Infix "um" inside.
    # We loop through word positions?
    # Or just check if 'infix' is in word. 
    for inf in infixes:
        if inf in word:
            # Try removing first occurrence
            # Usually infix is at start or after first consonant.
            # "b-um-ili" -> index 1.
            # "t-in-ago" -> index 1.
            # "gr-um-aduate" -> index 2?
            # Let's try removing it from anywhere for now? risky?
            # "summa" -> "sa"? No.
            # Only strictly if it splits index 0/1 or 1/2?
            # Let's try simple replacement first occurrence.
            
            # More rigorous: Check if word matches regex `^[^aeiou]+` + inf + `...`?
            # Simplest logic: replace and check root.
            root_cand = word.replace(inf, "", 1)
            # Check length to avoid "um" -> ""
            if len(root_cand) > 2 and root_cand in root_set:
                return True, f"infix({inf})", root_cand
                
    return False, None, None

def main():
    print("Loading affixes...")
    affix_data = load_json(AFFIX_FILE)
    affix_set = set()
    extract_affixes(affix_data, affix_set)
    prefixes, suffixes, infixes, circumfixes = categorize_affixes(affix_set)
    print(f"Found {len(affix_set)} unique affixes.")
    print(f"Prefixes: {len(prefixes)}, Suffixes: {len(suffixes)}, Infixes: {len(infixes)}, Circumfixes: {len(circumfixes)}")

    print("Loading dictionary roots...")
    all_roots = set()
    file_data = {}
    
    for fname in ROOT_FILES:
        data = load_json(fname)
        # data is list of dicts or dict? user might have cleaned it to list of dicts.
        # Check format
        words = []
        if isinstance(data, list):
            for entry in data:
                w = entry.get("word")
                if w: 
                    # normalize? lowercase?
                    w_norm = w.lower().strip()
                    words.append(entry) # Keep full entry
                    all_roots.add(w_norm)
        elif isinstance(data, dict):
            for w, entry in data.items():
                words.append({"word": w, "entry": entry})
                all_roots.add(w.lower().strip())
        
        file_data[fname] = words
        print(f"{fname}: {len(words)} entries loaded.")

    print(f"Total unique roots loaded: {len(all_roots)}")
    
    # Processing
    for fname, entries in file_data.items():
        print(f"Pruning {fname}...")
        kept = []
        removed_count = 0
        
        for entry in entries:
            word = entry["word"].lower().strip()
            
            # Skip short words?
            if len(word) <= 3:
                kept.append(entry)
                continue
                
            is_aff, description, root_found = is_affixed(word, all_roots, prefixes, suffixes, infixes, circumfixes)
            
            if is_aff and root_found != word: # avoid matching itself as root
                # "nagkaon" -> root "kaon". "kaon" is in all_roots. Remove "nagkaon".
                 removed_count += 1
                 # print(f"Removing {word} ({description}), found root: {root_found}")
            else:
                kept.append(entry)
                
        print(f"Removed {removed_count} affixed words from {fname}.")
        
        # Save back
        # If input was list, save as list.
        # file_data stored list of entries.
        save_json(fname, kept)

if __name__ == "__main__":
    main()
