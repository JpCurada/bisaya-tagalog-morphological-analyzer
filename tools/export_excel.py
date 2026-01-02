import json
import pandas as pd
import os

def load_roots(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return pd.DataFrame(data)

def flatten_affixes(node, category="General", flat_list=None):
    if flat_list is None:
        flat_list = []
        
    if isinstance(node, dict):
        # Check for affixes array
        if "affixes" in node:
            desc = node.get("description", category)
            for afx in node["affixes"]:
                entry = {"Category": desc}
                if isinstance(afx, str):
                    entry["Affix"] = afx
                    entry["Type"] = "Simple"
                elif isinstance(afx, dict):
                    entry["Affix"] = afx.get("affix", "")
                    entry["Type"] = "Detailed"
                    entry["Language"] = afx.get("language", "")
                    entry["Focus"] = afx.get("focus", "")
                    
                    # Flatten examples
                    examples = afx.get("examples", [])
                    ex_strs = []
                    for ex in examples:
                        ex_strs.append(f"{ex.get('root', '')} -> {ex.get('form', '')} ({ex.get('gloss', '')})")
                    entry["Examples"] = "; ".join(ex_strs)
                
                flat_list.append(entry)
        
        # Recurse
        for key, value in node.items():
            if key not in ["affixes", "examples"]:
                # If key allows us to refine category
                new_cat = category
                if isinstance(value, dict) or isinstance(value, list):
                     # simplistic folder name
                     pass 
                flatten_affixes(value, new_cat, flat_list)
                
    elif isinstance(node, list):
        for item in node:
            flatten_affixes(item, category, flat_list)
            
    return flat_list

def main():
    root_dir = "data"
    output_file = os.path.join(root_dir, "morphological_data.xlsx")
    
    # Files
    files = {
        "cebuano_roots_clean": "cebuano_roots_clean.json",
        "tagalog_roots_clean": "tagalog_roots_clean.json",
        "hiligaynon_roots_clean": "hiligaynon_roots_clean.json"
    }
    
    with pd.ExcelWriter(output_file, engine='openpyxl') as writer:
        # Process Roots
        for sheet_name, filename in files.items():
            path = os.path.join(root_dir, filename)
            if os.path.exists(path):
                print(f"Processing {filename}...")
                df = load_roots(path)
                df.to_excel(writer, sheet_name=sheet_name, index=False)
            else:
                print(f"Warning: {filename} not found.")

        # Process Unified Affixes
        affix_path = os.path.join(root_dir, "unified_bistag_affix.json")
        if os.path.exists(affix_path):
            print("Processing unified_bistag_affix.json...")
            with open(affix_path, 'r', encoding='utf-8') as f:
                affix_data = json.load(f)
            
            flat_affixes = flatten_affixes(affix_data)
            df_affix = pd.DataFrame(flat_affixes)
            df_affix.to_excel(writer, sheet_name="unified_bistag_affix", index=False)
            
    print(f"Export completed: {output_file}")

if __name__ == "__main__":
    main()
