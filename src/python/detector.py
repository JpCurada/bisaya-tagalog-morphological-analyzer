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
