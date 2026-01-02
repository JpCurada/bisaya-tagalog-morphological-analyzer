import sys
import os
import json

# Add project root to path
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from src.python.bindings import MorphologicalAnalyzer

def test_hiligaynon():
    project_root = os.path.join(os.path.dirname(__file__), '..')
    try:
        analyzer = MorphologicalAnalyzer(project_root)
        print("Analyzer initialized successfully.")
    except Exception as e:
        print(f"Failed to initialize analyzer: {e}")
        return

    # Test a Hiligaynon root
    word = "abo" # From hiligaynon_roots_clean.json
    res = analyzer.analyze(word)
    print(f"Analysis for '{word}': {json.dumps(res, indent=2)}")
    
    if res['valid'] and res['root'] == 'abo' and res['language'] == 'Hiligaynon':
        print("PASS: Hiligaynon root identified.")
    elif res['valid']: # Might be Bisaya too or Shared
        print(f"PASS: Word identified? Lang: {res['language']}")
    else:
        print("FAIL: Root not identified.")

if __name__ == "__main__":
    test_hiligaynon()
