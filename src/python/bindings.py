import ctypes
import os
import json
from .loader import load_data

# Define C Structures
class AnalysisResult(ctypes.Structure):
    _fields_ = [
        ("valid", ctypes.c_bool),
        ("word", ctypes.c_char * 256),
        ("prefix", ctypes.c_char * 64),
        ("root", ctypes.c_char * 128),
        ("suffix", ctypes.c_char * 64),
        ("language", ctypes.c_int), # Enum
        ("trace", (ctypes.c_char * 256) * 10),
        ("trace_count", ctypes.c_int)
    ]

# Language Enum Mapping
LANG_MAP = {
    0: "Unknown",
    1: "Bisaya",
    2: "Tagalog",
    3: "Shared" # Both
}

class MorphologicalAnalyzer:
    def __init__(self, project_root):
        lib_path = os.path.join(project_root, "lib", "libanalyzer.so")
        data_dir = os.path.join(project_root, "data")
        
        if not os.path.exists(lib_path):
             # Try building it? No, assume built.
             raise FileNotFoundError(f"Library not found at {lib_path}. Did you run 'make'?")
             
        self.lib = ctypes.CDLL(lib_path)
        
        # Setup signatures
        self.lib.init_analyzer.argtypes = [ctypes.c_char_p] * 5
        self.lib.analyze_word.argtypes = [ctypes.c_char_p, ctypes.POINTER(AnalysisResult)]
        self.lib.cleanup_analyzer.argtypes = []
        
        # Load Data
        data = load_data(data_dir)
        
        # Load JSON lexicons for meaning lookup
        self.lexicons = {}
        for name in ['bisaya_roots', 'tagalog_roots', 'shared_vocab', 'prefix_table', 'suffix_table']:
            json_path = os.path.join(data_dir, f"{name}.json")
            with open(json_path, 'r', encoding='utf-8') as f:
                self.lexicons[name] = json.load(f)
        
        # Initialize C Analyzer
        # Need to keep references to bytes to prevent garbage collection?
        # init_analyzer copies data? YES, populate_table calls my_strdup.
        self.lib.init_analyzer(
            data["prefix"].encode('utf-8'),
            data["suffix"].encode('utf-8'),
            data["bisaya"].encode('utf-8'),
            data["tagalog"].encode('utf-8'),
            data["shared"].encode('utf-8')
        )
        
    def analyze(self, word):
        res = AnalysisResult()
        # Analyze expects string. 
        # C uses simple char*, assumes null-terminated.
        b_word = word.encode('utf-8')
        self.lib.analyze_word(b_word, ctypes.byref(res))
        
        trace_msgs = []
        for i in range(res.trace_count):
            trace_msgs.append(res.trace[i].value.decode('utf-8', errors='replace'))
        
        # Decode components
        root = res.root.decode('utf-8', errors='replace') if res.root else None
        prefix = res.prefix.decode('utf-8', errors='replace') if res.prefix else None
        suffix = res.suffix.decode('utf-8', errors='replace') if res.suffix else None
        language = LANG_MAP.get(res.language, "Unknown")
        
        # Look up meaning and POS
        meaning = None
        pos = None
        origin = None
        
        if root:
            # Try to find in appropriate lexicon
            if language == "Bisaya" and root in self.lexicons['bisaya_roots']:
                meaning = self.lexicons['bisaya_roots'][root].get('meaning')
                pos = self.lexicons['bisaya_roots'][root].get('pos')
                origin = self.lexicons['bisaya_roots'][root].get('origin', 'Bisaya')
            elif language == "Tagalog" and root in self.lexicons['tagalog_roots']:
                meaning = self.lexicons['tagalog_roots'][root].get('meaning')
                pos = self.lexicons['tagalog_roots'][root].get('pos')
                origin = self.lexicons['tagalog_roots'][root].get('origin', 'Tagalog')
            elif language == "Shared" and root in self.lexicons['shared_vocab']:
                meaning = self.lexicons['shared_vocab'][root].get('meaning')
                pos = self.lexicons['shared_vocab'][root].get('pos')
                origin = self.lexicons['shared_vocab'][root].get('origin', 'Both')
            
        return {
            "word": res.word.decode('utf-8', errors='replace'),
            "valid": res.valid,
            "prefix": prefix,
            "root": root,
            "suffix": suffix,
            "language": language,
            "meaning": meaning,
            "pos": pos,
            "origin": origin,
            "trace": trace_msgs
        }
        
    def __del__(self):
        if hasattr(self, 'lib'):
            self.lib.cleanup_analyzer()
