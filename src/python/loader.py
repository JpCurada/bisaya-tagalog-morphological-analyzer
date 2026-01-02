import json
import os

def format_value(entry, default_lang=None):
    """
    Format: Language|POS|Meaning
    """
    lang = entry.get("language") or default_lang or "Unknown"
    pos = entry.get("pos") or "None"
    meaning = entry.get("definition") or entry.get("meaning") or "None"
    
    # Sanitize pipes in content
    pos = str(pos).replace("|", "/")
    meaning = str(meaning).replace("|", "/")
    
    return f"{lang}|{pos}|{meaning}"

def load_json_as_string(filepath, default_lang=None):
    """
    Load JSON (list of objects) and convert to "key|value\nkey2|value2" string
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Lexicon file not found: {filepath}")
        
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
        
    lines = []
    # Data is now a list of objects: [{"word": "foo", ...}, ...]
    if isinstance(data, list):
        for entry in data:
            key = entry.get("word")
            if key:
                val = format_value(entry, default_lang)
                lines.append(f"{key}|{val}")
    elif isinstance(data, dict):
        # Fallback for old format if needed, or error
        for key, entry in data.items():
            val = format_value(entry, default_lang)
            lines.append(f"{key}|{val}")
        
    return "\n".join(lines)

def load_data(data_dir):
    """
    Load data: Unified Affix JSON + Roots CSVs
    """
    # Load unified affix JSON content directly
    with open(os.path.join(data_dir, "unified_bistag_affix.json"), 'r', encoding='utf-8') as f:
        affix_json = f.read()

    return {
        "affix_json": affix_json,
        "bisaya": load_json_as_string(os.path.join(data_dir, "cebuano_roots_clean.json"), "Bisaya"),
        "tagalog": load_json_as_string(os.path.join(data_dir, "tagalog_roots_clean.json"), "Tagalog"),
        "hiligaynon": load_json_as_string(os.path.join(data_dir, "hiligaynon_roots_clean.json"), "Hiligaynon"),
        "shared": load_json_as_string(os.path.join(data_dir, "shared_vocab.json"), "Both")
    }
