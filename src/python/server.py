"""
Bistag Morphological Analyzer API Server
Modern Flask API with comprehensive endpoints for analysis and data browsing
"""
from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import os
import sys

# Add src to path to allow imports
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.python.bindings import MorphologicalAnalyzer
import logging

# Configure Logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    datefmt='%H:%M:%S'
)
logger = logging.getLogger('API')

app = Flask(__name__, 
            template_folder='../../web/templates',
            static_folder='../../web/static')

# Enable CORS for API endpoints
CORS(app, resources={r"/api/*": {"origins": "*"}})

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

# =====================================================
# Page Routes
# =====================================================

@app.route('/')
def landing():
    return render_template('landing.html')

@app.route('/analyzer')
def analyzer_page():
    return render_template('index.html')

@app.route('/browse')
def browse_page():
    return render_template('browse.html')

# =====================================================
# Analysis API
# =====================================================

@app.route('/api/analyze', methods=['POST'])
def analyze_text():
    """Analyze text and return morphological breakdown"""
    data = request.json
    text = data.get('text', '')
    
    if not text:
        return jsonify({"error": "No text provided"}), 400
        
    # Tokenize
    words = text.split()
    
    results = []
    ana = get_analyzer()
    
    for word in words:
        clean_word = word.strip(".,!?;:\"'()[]")
        if clean_word:
            res = ana.analyze(clean_word)
            res['original_token'] = word
            results.append(res)
        
    switches = detect_switches(results)
    stats = get_stats(results, switches)
    
    return jsonify({
        "results": results,
        "switches": switches,
        "stats": stats
    })

@app.route('/api/analyze/word/<word>', methods=['GET'])
def analyze_single_word(word):
    """Analyze a single word"""
    ana = get_analyzer()
    result = ana.analyze(word)
    return jsonify(result)

# =====================================================
# Data Browsing API
# =====================================================

@app.route('/api/stats', methods=['GET'])
def get_data_stats():
    """Get statistics about loaded data"""
    ana = get_analyzer()
    stats = ana.get_stats()
    # Add computed totals for frontend
    stats['total_affixes'] = stats['prefixes'] + stats['suffixes'] + stats['infixes'] + stats['circumfixes']
    stats['total_roots'] = stats['bisaya_roots'] + stats['tagalog_roots']
    return jsonify(stats)

@app.route('/api/affixes', methods=['GET'])
def get_all_affixes():
    """Get all affixes grouped by type"""
    ana = get_analyzer()
    return jsonify({
        "prefixes": ana.get_all_prefixes(),
        "suffixes": ana.get_all_suffixes(),
        "infixes": ana.get_all_infixes(),
        "circumfixes": ana.get_all_circumfixes()
    })

@app.route('/api/affixes/prefixes', methods=['GET'])
def get_prefixes():
    """Get all prefixes"""
    ana = get_analyzer()
    prefixes = ana.get_all_prefixes()
    # Format for frontend
    result = []
    for key, definitions in prefixes.items():
        result.append({
            "key": key,
            "affix_type": "prefix",
            "definitions": definitions,
            "count": len(definitions)
        })
    return jsonify(sorted(result, key=lambda x: x["key"]))

@app.route('/api/affixes/suffixes', methods=['GET'])
def get_suffixes():
    """Get all suffixes"""
    ana = get_analyzer()
    suffixes = ana.get_all_suffixes()
    result = []
    for key, definitions in suffixes.items():
        result.append({
            "key": key,
            "affix_type": "suffix",
            "definitions": definitions,
            "count": len(definitions)
        })
    return jsonify(sorted(result, key=lambda x: x["key"]))

@app.route('/api/affixes/infixes', methods=['GET'])
def get_infixes():
    """Get all infixes"""
    ana = get_analyzer()
    infixes = ana.get_all_infixes()
    result = []
    for key, definitions in infixes.items():
        result.append({
            "key": key,
            "affix_type": "infix",
            "definitions": definitions,
            "count": len(definitions)
        })
    return jsonify(sorted(result, key=lambda x: x["key"]))

@app.route('/api/affixes/circumfixes', methods=['GET'])
def get_circumfixes():
    """Get all circumfixes"""
    ana = get_analyzer()
    circumfixes = ana.get_all_circumfixes()
    result = []
    for key, definitions in circumfixes.items():
        result.append({
            "key": key,
            "affix_type": "circumfix",
            "definitions": definitions,
            "count": len(definitions)
        })
    return jsonify(sorted(result, key=lambda x: x["key"]))

@app.route('/api/affix/<affix_type>/<path:key>', methods=['GET'])
def get_affix_detail(affix_type, key):
    """Get detailed information about a specific affix"""
    ana = get_analyzer()
    
    if affix_type == "prefix":
        info = ana.get_prefix_info(key)
    elif affix_type == "suffix":
        info = ana.get_suffix_info(key)
    else:
        return jsonify({"error": "Invalid affix type"}), 400
    
    if info:
        return jsonify(info)
    return jsonify({"error": "Affix not found"}), 404

# =====================================================
# Root Browsing API
# =====================================================

@app.route('/api/roots/search', methods=['GET'])
def search_roots():
    """Search roots by query"""
    query = request.args.get('q', '')
    language = request.args.get('language', None)
    limit = int(request.args.get('limit', 50))
    
    if not query:
        return jsonify({"error": "Query parameter 'q' required"}), 400
    
    ana = get_analyzer()
    results = ana.search_roots(query, language, limit)
    return jsonify(results)

@app.route('/api/roots/<path:word>', methods=['GET'])
def get_root_detail(word):
    """Get detailed information about a specific root word"""
    language = request.args.get('language', None)
    ana = get_analyzer()
    info = ana.lookup_root(word, language)
    
    if info:
        return jsonify(info)
    return jsonify({"error": "Root not found"}), 404

@app.route('/api/roots/random', methods=['GET'])
def get_random_roots():
    """Get random sample of roots for display"""
    import random
    count = int(request.args.get('count', 10))
    language = request.args.get('language', None)
    
    ana = get_analyzer()
    results = []
    
    # Sample from each dictionary
    if language in (None, "Bisaya"):
        bisaya_keys = list(ana.bisaya_roots.keys())
        sample_size = min(count // 2 if language is None else count, len(bisaya_keys))
        for key in random.sample(bisaya_keys, sample_size):
            info = ana.bisaya_roots[key]
            results.append({**info, "source": "Bisaya"})
    
    if language in (None, "Tagalog"):
        tagalog_keys = list(ana.tagalog_roots.keys())
        sample_size = min(count // 2 if language is None else count, len(tagalog_keys))
        for key in random.sample(tagalog_keys, sample_size):
            info = ana.tagalog_roots[key]
            results.append({**info, "source": "Tagalog"})
    
    random.shuffle(results)
    return jsonify(results[:count])

# =====================================================
# Main Entry Point
# =====================================================

if __name__ == '__main__':
    try:
        get_analyzer()
        print("\n" + "=" * 50)
        print("  Bistag Morphological Analyzer API")
        print("  Starting web server on port 8000...")
        print("=" * 50 + "\n")
        app.run(host='0.0.0.0', port=8000, debug=False)
    except Exception as e:
        print(f"Error starting server: {e}")
