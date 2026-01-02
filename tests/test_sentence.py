import sys
import os
import json

# Add project root to path
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from src.python.bindings import MorphologicalAnalyzer

def analyze_sentence(analyzer, sentence):
    words = sentence.split() # Simple whitespace splitting
    results = []
    for word in words:
        # Strip punctuation?
        norm_word = word.strip(".,?!")
        res = analyzer.analyze(norm_word)
        results.append(res)
    return results

def test_rich_output():
    project_root = os.path.join(os.path.dirname(__file__), '..')
    try:
        analyzer = MorphologicalAnalyzer(project_root)
        print("Analyzer initialized.")
    except Exception as e:
        print(f"Failed to init: {e}")
        return

    sentence = "nagkaon ka na lab?"
    print(f"Analyzing: '{sentence}'")
    
    results = analyze_sentence(analyzer, sentence)
    
    for res in results:
        print("-" * 20)
        print(f"Word: {res['word']}")
        if res['valid']:
            print(f"Root: {res['root']}")
            print(f"Prefix: {res['prefix']} ({res['prefix_info']})")
            print(f"Suffix: {res['suffix']} ({res['suffix_info']})")
            print(f"Root Info: {res['root_pos']} - {res['root_definition']}")
            print(f"Language: {res['language']}")
        else:
            print("Valid: False")
        
        print("Trace:")
        for t in res['trace']:
            print(f"  {t}")

if __name__ == "__main__":
    test_rich_output()
