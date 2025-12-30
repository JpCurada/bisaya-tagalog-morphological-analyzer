from flask import Flask, request, jsonify, render_template
import os
import sys

# Add src to path to allow imports
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.python.bindings import MorphologicalAnalyzer
from src.python.detector import detect_switches, get_stats

app = Flask(__name__, 
            template_folder='../../web/templates',
            static_folder='../../web/static')

# Global Analyzer Instance
analyzer = None

def get_analyzer():
    global analyzer
    if analyzer is None:
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
        try:
            analyzer = MorphologicalAnalyzer(root_dir)
            print("Analyzer initialized successfully.")
        except Exception as e:
            print(f"Failed to initialize analyzer: {e}")
            raise e
    return analyzer

@app.route('/')
def landing():
    return render_template('landing.html')

@app.route('/analyzer')
def analyzer_page():
    return render_template('index.html')

@app.route('/api/analyze', methods=['POST'])
def analyze_text():
    data = request.json
    text = data.get('text', '')
    
    if not text:
        return jsonify({"error": "No text provided"}), 400
        
    # Simple tokenization (split by whitespace)
    # TODO: Better tokenization (handle punctuation)
    words = text.split()
    
    results = []
    ana = get_analyzer()
    
    for word in words:
        # Strip simple punctuation for analysis if needed?
        # For now, pass raw word. Analyzer handles exact matches.
        # If user types "kaon.", "kaon" won't match.
        # Let's do simple strip of trailing punctuation for better UX.
        clean_word = word.strip(".,!?;:\"")
        res = ana.analyze(clean_word)
        # Add original word back to result for display consistency?
        # The C analyzer returns the word passed to it.
        # Let's attach the original token just in case.
        res['original_token'] = word 
        results.append(res)
        
    switches = detect_switches(results)
    stats = get_stats(results, switches)
    
    return jsonify({
        "results": results,
        "switches": switches,
        "stats": stats
    })

if __name__ == '__main__':
    # Initialize implementation
    try:
        get_analyzer()
        print("Starting web server on port 8000...")
        app.run(port=8000, debug=True)
    except Exception as e:
        print(f"Error starting server: {e}")
