import json
import re
import os
import glob

DATA_DIR = "data_v2"
FILES = [
    "prefix_table.json",
    "suffix_table.json",
    "infix_table.json",
    "circumfix_table.json"
]

def clean_text(text):
    return re.sub(r'\s+', ' ', text).strip()

def parse_derived_terms(text_block, current_language):
    # Try to find the clean list line
    # "Tagalog terms prefixed with ..."
    regex = rf"{current_language} terms prefixed with [^\n]+"
    matches = re.findall(regex, text_block)
    
    terms = []
    if matches:
        # Take the last match? Or all?
        for match in matches:
            # The line usually is "Tagalog terms prefixed with X term1 term2 term3"
            # We remove the prefix phrase
            prefix_pattern = rf"{current_language} terms prefixed with \S+\s*"
            content = re.sub(prefix_pattern, "", match)
            # Split by space
            found = content.split()
            terms.extend(found)
    else:
        # Fallback: look for bullets?
        # But looking at ma- example, it might be mashed. 
        # If no clean line found, return empty or try heuristic
        pass
        
    return list(set(terms)) # Deduplicate

def parse_entry(entry):
    # If etymology is just a clean string, leave it?
    # But currently it is a BLOB.
    blob = entry.get("etymology", "")
    if not blob or "Etymology" not in blob and "Pronunciation" not in blob:
        # Might be already clean or empty
        return entry

    # It's a blob. Let's parse it.
    lines = blob.split('\n')
    
    parsed = {
        "etymology": [],
        "pronunciation": [],
        "syllabification": [],
        "function": [],
        "derived_terms": []
    }
    
    current_section = "etymology" 
    
    # regex for headers
    header_re = re.compile(r'^(Etymology|Pronunciation|Syllabification|Prefix|Suffix|Infix|Circumfix|Derived terms|Adjective|Verb|Noun|Usage notes)(\s*\d+)?\[edit\]', re.IGNORECASE)
    
    for line in lines:
        line = line.strip()
        if not line: continue
        
        match = header_re.match(line)
        if match:
            head = match.group(1).lower()
            if "etymology" in head:
                current_section = "etymology"
            elif "pronunciation" in head:
                current_section = "pronunciation"
            elif "syllabification" in head:
                current_section = "syllabification"
            elif "derived terms" in head:
                current_section = "derived_terms"
            elif any(x in head for x in ["prefix", "suffix", "infix", "circumfix", "adjective", "verb", "noun"]):
                current_section = "function"
            elif "usage" in head:
                current_section = "function" # Append usage notes to function
            else:
                current_section = "other"
            continue
            
        # Add content to section
        if current_section == "etymology":
             # Skip "Etymology tree..." lines if they are garbage?
             # User example: "From Proto-Philippine *di."
             parsed["etymology"].append(line)
             
        elif current_section == "pronunciation":
             parsed["pronunciation"].append(line)
             # Also check for Syllabification inside Pronunciation section (common)
             if "Syllabification:" in line:
                 parsed["syllabification"].append(line)
                 
        elif current_section == "syllabification":
             parsed["syllabification"].append(line)
             
        elif current_section == "function":
             # Skip the Baybayin line if it's just the word repeated?
             # User example: "dî- (Baybayin spelling ᜇᜒ)" -> This is effectively the headword line.
             # We might want to keep it or skip it.
             # User target: "non-; un-; de"
             # So we skip the headword line.
             if "Baybayin spelling" in line or "Badlit spelling" in line:
                 continue
             parsed["function"].append(line)
             
        elif current_section == "derived_terms":
             parsed["derived_terms"].append(line)

    # Post-process
    
    # Etymology: Join lines. Remove "Etymology tree..." if messy?
    # User wanted "From Proto..."
    etym_text = "\n".join(parsed["etymology"]).strip()
    # Clean up "Etymology tree..." artifacts?
    if "Etymology tree" in etym_text:
        # Naive: try to find the "From..." sentence
        match_from = re.search(r'(From\s+.*|Inherited\s+.*|Borrowed\s+.*)', etym_text)
        if match_from:
            etym_text = match_from.group(0) # Take start of that match to end?
            # Or just keep it all but clean
    
    # Pronunciation: Extract IPA
    pron_text = "\n".join(parsed["pronunciation"])
    ipa_match = re.search(r'IPA\(key\): ([^\n]+)', pron_text)
    ipa_val = ipa_match.group(0) if ipa_match else None # Keep the whole "IPA(key): /.../" string as per user example? 
    # User example: "(Standard Tagalog) IPA(key): /ˈdiʔ/ [ˈd̪ɪʔ]" -> Yes, keep it.
    if not ipa_val and pron_text:
         # Find line with IPA
         for l in parsed["pronunciation"]:
             if "IPA" in l:
                 ipa_val = l
                 break
    
    # Syllabification
    syl_list = parsed["syllabification"]
    syl_val = ""
    for s in syl_list:
        if "Syllabification" in s:
            # Extract value "di-"
            # User example: "Syllabification: di-" -> "di"
            parts = s.split(":")
            if len(parts) > 1:
                syl_val = parts[1].strip().replace("-", "")
            break
            
    # Function
    func_text = "; ".join(parsed["function"]).strip()
    # Clean up
    func_text = re.sub(r' +', ' ', func_text)
    
    # Derived Terms
    # Parse the buffer we collected
    derived_blob = "\n".join(parsed["derived_terms"])
    dt_list = parse_derived_terms(derived_blob, entry.get("language", "Tagalog"))
    
    # Construct new entry
    new_entry = {
        "language": entry["language"],
        "etymology": etym_text,
        "pronunciation": ipa_val,
        "syllabification": syl_val,
        "function": func_text,
        "derived_terms": dt_list
    }
    return new_entry

def process_file(filename):
    path = os.path.join(DATA_DIR, filename)
    if not os.path.exists(path):
        print(f"File not found: {path}")
        return

    print(f"Processing {filename}...")
    with open(path, 'r', encoding='utf-8') as f:
        data = json.load(f)
        
    cleaned_data = {}
    
    for affix, entries in data.items():
        cleaned_entries = []
        for entry in entries:
            try:
                cleaned_entries.append(parse_entry(entry))
            except Exception as e:
                print(f"Error parsing {affix}: {e}")
                cleaned_entries.append(entry) # Fallback
        
        cleaned_data[affix] = cleaned_entries
        
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(cleaned_data, f, ensure_ascii=False, indent=2)
    print(f"Saved {filename}")

if __name__ == "__main__":
    for fname in FILES:
        process_file(fname)
