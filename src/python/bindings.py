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
        ("prefix_info", ctypes.c_char * 256),
        ("root", ctypes.c_char * 128),
        ("root_pos", ctypes.c_char * 64),
        ("root_definition", ctypes.c_char * 1024),
        ("suffix", ctypes.c_char * 64),
        ("suffix_info", ctypes.c_char * 256),
        ("language", ctypes.c_int), # Enum
        ("trace", (ctypes.c_char * 256) * 10),
        ("trace_count", ctypes.c_int)
    ]
    
    def to_dict(self):
        return {
            "valid": self.valid,
            "word": self.word.decode('utf-8', errors='ignore'),
            "prefix": self.prefix.decode('utf-8', errors='ignore'),
            "prefix_info": self.prefix_info.decode('utf-8', errors='ignore'),
            "root": self.root.decode('utf-8', errors='ignore'),
            "root_pos": self.root_pos.decode('utf-8', errors='ignore'),
            "root_definition": self.root_definition.decode('utf-8', errors='ignore'),
            "suffix": self.suffix.decode('utf-8', errors='ignore'),
            "suffix_info": self.suffix_info.decode('utf-8', errors='ignore'),
            "language": ["Unknown", "Cebuano", "Tagalog", "Hiligaynon", "Both"][self.language] if 0 <= self.language <= 4 else "Unknown",
            "trace": [self.trace[i].decode('utf-8', errors='ignore') for i in range(self.trace_count)]
        }

# Language Enum Mapping
LANG_MAP = {
    0: "Unknown",
    1: "Bisaya",
    2: "Tagalog",
    3: "Shared" # Both
}

class MorphologicalAnalyzer:
    def __init__(self, project_root):
        if os.name == 'nt':
            lib_name = "libanalyzer_v2.dll"
        else:
            lib_name = "libanalyzer.so"
            
        lib_path = os.path.join(project_root, "lib", lib_name)
        data_dir = os.path.join(project_root, "data")
        
        if not os.path.exists(lib_path):
            # Fallback for previous build name if necessary, or just fail
            if os.path.exists(os.path.join(project_root, "lib", "libanalyzer.so")):
                 lib_path = os.path.join(project_root, "lib", "libanalyzer.so")
            else:
                 raise FileNotFoundError(f"Library not found at {lib_path}. Did you run 'make'?")
             
        self.lib = ctypes.CDLL(lib_path)
        
        # Setup signatures
        # Setup signatures
        self.lib.init_analyzer.argtypes = [ctypes.c_char_p] * 4 # affix_json, bisaya, tagalog, shared
        self.lib.analyze_word.argtypes = [ctypes.c_char_p, ctypes.POINTER(AnalysisResult)]
        self.lib.cleanup_analyzer.argtypes = []
        
        # Load Data
        # Load JSON lexicons for meaning lookup
        self.lexicons = {}
        for name in ['bisaya_roots', 'tagalog_roots', 'shared_vocab']:
            json_path = os.path.join(data_dir, f"{name}.json")
            with open(json_path, 'r', encoding='utf-8') as f:
                self.lexicons[name] = json.load(f)
        
        # Initialize
        self.lib.init_analyzer.argtypes = [ctypes.c_char_p] * 5 # affix_json, bisaya, tagalog, hiligaynon, shared
        self.lib.init_analyzer.restype = None
        
        # Load data
        print("Loading lexicon data...")
        data = load_data(data_dir)
        
        self.lib.init_analyzer(
            data["affix_json"].encode('utf-8'),
            data["bisaya"].encode('utf-8'),
            data["tagalog"].encode('utf-8'),
            data["hiligaynon"].encode('utf-8'), # Added Hiligaynon
            data["shared"].encode('utf-8')
        )
        
    def analyze(self, word):
        res = AnalysisResult()
        # Analyze expects string. 
        # C uses simple char*, assumes null-terminated.
        b_word = word.encode('utf-8')
        self.lib.analyze_word(b_word, ctypes.byref(res))
        
        # Decode components
        root = res.root.decode('utf-8', errors='replace') if res.root else None
        prefix = res.prefix.decode('utf-8', errors='replace') if res.prefix else None
        prefix_info = res.prefix_info.decode('utf-8', errors='replace') if res.prefix_info else None
        suffix = res.suffix.decode('utf-8', errors='replace') if res.suffix else None
        suffix_info = res.suffix_info.decode('utf-8', errors='replace') if res.suffix_info else None
        
        # Root Info from C
        root_pos = res.root_pos.decode('utf-8', errors='replace') if res.root_pos else None
        root_def = res.root_definition.decode('utf-8', errors='replace') if res.root_definition else None
        
        language = LANG_MAP.get(res.language, "Unknown")
        
        trace_msgs = []
        for i in range(res.trace_count):
            trace_msgs.append(res.trace[i].value.decode('utf-8', errors='replace'))
            
        return {
            "word": res.word.decode('utf-8', errors='replace'),
            "valid": res.valid,
            "prefix": prefix,
            "prefix_info": prefix_info,
            "root": root,
            "root_pos": root_pos,
            "root_definition": root_def,
            "suffix": suffix,
            "suffix_info": suffix_info,
            "language": language,
            "trace": trace_msgs
        }
        
    def __del__(self):
        if hasattr(self, 'lib'):
            self.lib.cleanup_analyzer()
