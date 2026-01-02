import json
import re
import os

# Paths
DATA_DIR = r'c:\Users\JP CURADA\bisaya-tagalog-morphological-analyzer\data'
UNIFIED_AFFIX = os.path.join(DATA_DIR, 'unified_bistag_affix.json')
CEB_DICT = os.path.join(DATA_DIR, 'cebuano_dictionary.json')
TAG_DICT = os.path.join(DATA_DIR, 'tagalog_dictionary.json')
HIL_DICT = os.path.join(DATA_DIR, 'hiligaynon_dictionary.json')

# Output paths
CEB_OUT = os.path.join(DATA_DIR, 'cebuano_roots_clean.json')
TAG_OUT = os.path.join(DATA_DIR, 'tagalog_roots_clean.json')
HIL_OUT = os.path.join(DATA_DIR, 'hiligaynon_roots_clean.json')

def load_json(path):
    print(f"Attempting to load: {path}")
    try:
        with open(path, 'r', encoding='utf-8') as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        print(f"ERROR: Failed to decode JSON from {path}")
        print(f"Error details: {e}")
        return []
    except Exception as e:
        print(f"ERROR: An unexpected error occurred while loading {path}")
        print(f"Error details: {e}")
        return []

def save_json(data, path):
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    print(f"Saved {len(data)} entries to {path}")

def extract_affixes(data):
    affixes = set()
    
    def traverse(node):
        if isinstance(node, dict):
            for k, v in node.items():
                if k == 'affixes':
                    if isinstance(v, list):
                        for item in v:
                            if isinstance(item, str):
                                affixes.add(item)
                            elif isinstance(item, dict) and 'affix' in item:
                                affixes.add(item['affix'])
                            elif isinstance(item, dict) and 'affixes' in item: # specialized structure
                                pass # Should be caught by recursion if structure allows, but let's be safe
                elif k == 'affix' and isinstance(v, str):
                    affixes.add(v)
                else:
                    traverse(v)
        elif isinstance(node, list):
            for item in node:
                traverse(item)

    traverse(data)
    
    # Process affixes to match dictionary entries
    # 1. Strip hyphens for direct matching (many dicts verify affixes without hyphens or with them)
    # 2. Keep original forms too
    processed = set()
    for af in affixes:
        processed.add(af)
        processed.add(af.replace('-', ''))
    
    return processed

def clean_dictionary(entries, lang, affixes_set):
    cleaned = []
    removed_count = 0
    
    for entry in entries:
        word = entry.get('word', '').strip()
        definition = entry.get('definition', '').lower()
        
        # Check 1: Is the word an affix?
        if word in affixes_set:
            removed_count += 1
            # print(f"Removed affix entry: {word}")
            continue
            
        # Check 2: Language specific filters
        if lang == 'Cebuano':
            # Remove words with "affix" in definition
            if 'affix' in definition:
                removed_count += 1
                continue
                
        elif lang == 'Tagalog':
            # Remove "v., inf" (verb, infinitive - usually affixed)
            if 'v., inf' in definition:
                removed_count += 1
                continue
                
        elif lang == 'Hiligaynon':
             # Assume "affix" removal similar to Cebuano
            if 'affix' in definition:
                removed_count += 1
                continue

        cleaned.append(entry)
        
    print(f"[{lang}] Removed {removed_count} entries. Remaining: {len(cleaned)}")
    return cleaned

def main():
    print("Loading unified affix file...")
    unified_data = load_json(UNIFIED_AFFIX)
    affix_set = extract_affixes(unified_data)
    print(f"Extracted {len(affix_set)} unique affix forms.")

    # Cebuano
    if os.path.exists(CEB_DICT):
        print("\nProcessing Cebuano...")
        ceb_data = load_json(CEB_DICT)
        ceb_clean = clean_dictionary(ceb_data, 'Cebuano', affix_set)
        save_json(ceb_clean, CEB_OUT)
    
    # Tagalog
    if os.path.exists(TAG_DICT):
        print("\nProcessing Tagalog...")
        tag_data = load_json(TAG_DICT)
        tag_clean = clean_dictionary(tag_data, 'Tagalog', affix_set)
        save_json(tag_clean, TAG_OUT)

    # Hiligaynon
    if os.path.exists(HIL_DICT):
        print("\nProcessing Hiligaynon...")
        hil_data = load_json(HIL_DICT)
        hil_clean = clean_dictionary(hil_data, 'Hiligaynon', affix_set)
        save_json(hil_clean, HIL_OUT)

if __name__ == "__main__":
    main()
