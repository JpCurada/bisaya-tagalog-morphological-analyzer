import json
import os

def format_value(entry, default_lang=None):
    """
    Convert dictionary entry to pipe-delimited string: "Language|POS|Extra"
    """
    lang = entry.get("language", default_lang)
    pos = entry.get("compatible_pos") or entry.get("pos")
    
    # Handle list of POS
    if isinstance(pos, list):
        pos_str = ",".join(pos)
    else:
        pos_str = str(pos)
        
    extra = entry.get("function") or entry.get("meaning") or ""
    
    return f"{lang}|{pos_str}|{extra}"

def load_json_as_string(filepath, default_lang=None):
    """
    Load JSON and convert to "key|value\nkey2|value2" string
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Lexicon file not found: {filepath}")
        
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
        
    lines = []
    for key, entry in data.items():
        val = format_value(entry, default_lang)
        lines.append(f"{key}|{val}")
        
    return "\n".join(lines)

def load_data(data_dir):
    """
    Load all 5 lexicon files and return raw strings
    """
    return {
        "prefix": load_json_as_string(os.path.join(data_dir, "prefix_table.json")),
        "suffix": load_json_as_string(os.path.join(data_dir, "suffix_table.json")),
        "bisaya": load_json_as_string(os.path.join(data_dir, "bisaya_roots.json"), "Bisaya"),
        "tagalog": load_json_as_string(os.path.join(data_dir, "tagalog_roots.json"), "Tagalog"),
        "shared": load_json_as_string(os.path.join(data_dir, "shared_vocab.json"), "Both")
    }
