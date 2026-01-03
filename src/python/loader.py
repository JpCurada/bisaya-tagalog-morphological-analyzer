import json
import os

def format_value_from_dict(entry, default_lang=None):
    """
    Convert dictionary entry to pipe-delimited string: "Language|POS|Extra"
    Used for affix tables and shared_vocab (which remain in dict format)
    """
    lang = entry.get("language", default_lang)
    pos = entry.get("compatible_pos") or entry.get("pos")
    
    # Handle list of POS
    if isinstance(pos, list):
        pos_str = ",".join(pos)
    else:
        pos_str = str(pos) if pos else "unknown"
        
    extra = entry.get("function") or entry.get("meaning") or ""
    
    return f"{lang}|{pos_str}|{extra}"

def format_value_from_array_item(item, default_lang=None):
    """
    Convert array item (new root format) to pipe-delimited string: "Language|POS|Definition"
    Used for bisaya_roots.json and tagalog_roots.json (array format)
    """
    lang = item.get("language", default_lang)
    pos = item.get("part_of_speech") or "unknown"
    definition = item.get("definition", "")
    
    # Truncate definition if too long (for C compat)
    if len(definition) > 200:
        definition = definition[:200] + "..."
    
    return f"{lang}|{pos}|{definition}"

def load_affix_json_as_string(filepath, default_lang=None):
    """
    Load affix JSON (object format where each key maps to array of definitions)
    and convert to "key|value\nkey2|value2" string for legacy C loader
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Affix file not found: {filepath}")
        
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
        
    lines = []
    for key, entries in data.items():
        # entries is a list of definitions
        if isinstance(entries, list) and len(entries) > 0:
            # Use first entry for simple compatibility
            first_entry = entries[0]
            lang = first_entry.get("language", default_lang)
            pos = first_entry.get("focus") or first_entry.get("type") or ""
            func = first_entry.get("function", "")
            val = f"{lang}|{pos}|{func}"
        else:
            val = f"{default_lang}|unknown|"
        lines.append(f"{key}|{val}")
        
    return "\n".join(lines)

def load_root_json_as_string(filepath, default_lang=None):
    """
    Load root JSON (array format: [{word, definition, language, part_of_speech}, ...])
    and convert to "word|value\nword2|value2" string for legacy C loader
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Root lexicon file not found: {filepath}")
        
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # Handle array format
    if isinstance(data, list):
        lines = []
        seen = set()  # Avoid duplicates (same word can appear multiple times)
        for item in data:
            word = item.get("word", "")
            if word and word not in seen:
                val = format_value_from_array_item(item, default_lang)
                lines.append(f"{word}|{val}")
                seen.add(word)
        return "\n".join(lines)
    else:
        # Fallback to dict format (like shared_vocab)
        lines = []
        for key, entry in data.items():
            val = format_value_from_dict(entry, default_lang)
            lines.append(f"{key}|{val}")
        return "\n".join(lines)

def load_shared_vocab_as_string(filepath, default_lang="Both"):
    """
    Load shared_vocab.json (object format: {word: {meaning, pos, origin}})
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"Shared vocab file not found: {filepath}")
        
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    lines = []
    for key, entry in data.items():
        lang = entry.get("origin", default_lang)
        pos = entry.get("pos", "unknown")
        meaning = entry.get("meaning", "")
        val = f"{lang}|{pos}|{meaning}"
        lines.append(f"{key}|{val}")
        
    return "\n".join(lines)

def load_data(data_dir):
    """
    Load all lexicon files and return raw strings for C analyzer initialization
    """
    return {
        "prefix": load_affix_json_as_string(os.path.join(data_dir, "prefix_table.json")),
        "suffix": load_affix_json_as_string(os.path.join(data_dir, "suffix_table.json")),
        "bisaya": load_root_json_as_string(os.path.join(data_dir, "bisaya_roots.json"), "Bisaya"),
        "tagalog": load_root_json_as_string(os.path.join(data_dir, "tagalog_roots.json"), "Tagalog"),
        "shared": load_shared_vocab_as_string(os.path.join(data_dir, "shared_vocab.json"), "Both")
    }

# New functions for loading JSON data directly (for Python-side lookups)

def load_bisaya_roots(filepath):
    """Load bisaya roots as a dictionary keyed by word"""
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    result = {}
    for item in data:
        word = item.get("word", "")
        if word:
            result[word.lower()] = {
                "word": word,
                "definition": item.get("definition", ""),
                "language": item.get("language", "Hiligaynon"),
                "pos": item.get("part_of_speech"),
                "link": item.get("link", "")
            }
    return result

def load_tagalog_roots(filepath):
    """Load tagalog roots as a dictionary keyed by word"""
    with open(filepath, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    result = {}
    for item in data:
        word = item.get("word", "")
        if word:
            result[word.lower()] = {
                "word": word,
                "definition": item.get("definition", ""),
                "language": item.get("language", "Tagalog"),
                "pos": item.get("part_of_speech"),
                "link": item.get("link", "")
            }
    return result

def load_shared_vocab(filepath):
    """Load shared vocabulary as-is (already in dict format)"""
    with open(filepath, 'r', encoding='utf-8') as f:
        return json.load(f)
