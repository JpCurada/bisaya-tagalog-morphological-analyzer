import json
import os

def load_json_file(filepath):
    """Generic helper to load a JSON file safely"""
    try:
        if os.path.exists(filepath):
            with open(filepath, 'r', encoding='utf-8') as f:
                return json.load(f)
        return {}
    except Exception as e:
        print(f"Warning: Failed to load {filepath}: {e}")
        return {}

def load_affix_table(filepath):
    """Load an affix table JSON file (prefix, suffix, infix, circumfix)"""
    return load_json_file(filepath)

def load_bisaya_roots(filepath):
    """Load bisaya roots as a dictionary keyed by word"""
    data = load_json_file(filepath)
    if not data:
        return {}
        
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
    data = load_json_file(filepath)
    if not data:
        return {}
    
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


