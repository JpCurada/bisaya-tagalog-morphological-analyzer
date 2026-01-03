
import csv
import json
import os
import sys

def generate_affix_tables():
    input_file = 'data/affixes.csv'
    
    # Tables to populate
    tables = {
        'Prefix': {},
        'Suffix': {},
        'Infix': {},
        'Circumfix': {}
    }

    # Map for cleaner filenames
    file_map = {
        'Prefix': 'data/prefix_table.json',
        'Suffix': 'data/suffix_table.json',
        'Infix': 'data/infix_table.json',
        'Circumfix': 'data/circumfix_table.json'
    }

    if not os.path.exists(input_file):
        print(f"Error: {input_file} not found.")
        return

    try:
        with open(input_file, mode='r', encoding='utf-8-sig') as csvfile:
            reader = csv.DictReader(csvfile)
            
            for i, row in enumerate(reader):
                try:
                    if not row:
                        continue
                    
                    # specific check for empty rows
                    if not row.get('Position'):
                        print(f"Skipping row {i+2} (missing Position): {row}")
                        continue

                    position = row.get('Position', '').strip()
                    affix = row.get('Affix', '').strip()
                    
                    if not position or not affix:
                        continue

                    if position not in tables:
                        # Handle unexpected positions if any, or skip
                        print(f"Warning: Unknown position '{position}' in row {i+2}")
                        continue

                    # Construct the entry
                    entry = {
                        "type": row.get('Type'),
                        "language": row.get('Language'),
                        "focus": row.get('Voice_Focus'),
                        "aspect": row.get('Aspect'),
                        "tense": row.get('Tense'),
                        "function": row.get('Function_Meaning'),
                        "rules": row.get('Notes'),
                        "allomorphs": row.get('Allomorphs'),
                        "example": {
                            "root_ceb": row.get('Example_Root_CEB'),
                            "form_ceb": row.get('Example_Form_CEB'),
                            "gloss_ceb": row.get('Example_Gloss_CEB'),
                            "root_tag": row.get('Example_Root_TAG'),
                            "form_tag": row.get('Example_Form_TAG'),
                            "gloss_tag": row.get('Example_Gloss_TAG')
                        }
                    }
                    
                    # Remove empty fields to keep JSON clean
                    entry = {k: v for k, v in entry.items() if v and str(v).strip() != ""}
                    if "example" in entry:
                         entry["example"] = {k: v for k, v in entry["example"].items() if v and str(v).strip() != ""}
                         if not entry["example"]:
                             del entry["example"]

                    if affix not in tables[position]:
                        tables[position][affix] = []
                    
                    tables[position][affix].append(entry)

                except Exception as e:
                    print(f"Error processing row {i+2}: {row}")
                    print(f"Exception: {e}")
                    # convert row to dict if it isn't slightly
                    raise e

        # Write to files
        for position, content in tables.items():
            output_file = file_map.get(position)
            if output_file:
                with open(output_file, 'w', encoding='utf-8') as f:
                    json.dump(content, f, indent=4, ensure_ascii=False)
                print(f"Created {output_file} with {len(content)} keys.")
    
    except Exception as e:
        print(f"Fatal error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    generate_affix_tables()
