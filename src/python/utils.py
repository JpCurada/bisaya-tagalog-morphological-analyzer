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

def detect_switches(analyses):
    """
    Detect code-switching points in a sequence of word analyses.
    
    Args:
        analyses (list): List of dicts returned by analyzer.analyze()
        
    Returns:
        list: List of switch events
    """
    switches = []
    current_lang = None
    
    for i, res in enumerate(analyses):
        lang = res['language']
        
        # We only consider definitive languages for switching context
        if lang in ['Bisaya', 'Tagalog']:
            if current_lang and current_lang != lang:
                switches.append({
                    'index': i,
                    'word': res['word'],
                    'from': current_lang,
                    'to': lang
                })
            current_lang = lang
            
    return switches

def get_stats(analyses, switches):
    """
    Generate summary statistics
    """
    total = len(analyses)
    valid = sum(1 for a in analyses if a['valid'])
    langs = [a['language'] for a in analyses]
    
    return {
        "total_words": total,
        "valid_words": valid,
        "switch_count": len(switches),
        "bisaya_count": langs.count("Bisaya"),
        "tagalog_count": langs.count("Tagalog"),
        "shared_count": langs.count("Shared"),
        "unknown_count": langs.count("Unknown")
    }

