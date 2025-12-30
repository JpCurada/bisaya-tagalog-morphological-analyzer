# Comprehensive Implementation Plan
## Bisaya-Tagalog Code-Switching Morphological Analyzer Using Finite Automata

---

## I. PROJECT OVERVIEW

### A. Core Objective
Develop a hybrid C/Python system that performs morphological analysis and code-switching detection on mixed Bisaya-Tagalog text using Deterministic Finite Automata (DFA) with integrated lookup table architecture.

### B. Implementation Strategy
**Hybrid Architecture Approach:**
- **C Language**: Implements core computational logic (DFA state machine, morphological parsing algorithms, hash table data structures)
- **Python Language**: Handles auxiliary functions (data loading, format conversion, web server, API endpoints, testing infrastructure)
- **Communication Layer**: Python-to-C interface using ctypes foreign function interface (FFI)

### C. Rationale for Hybrid Approach
1. **Academic Requirement Satisfaction**: Demonstrates low-level systems programming and automata implementation in C
2. **Development Efficiency**: Leverages Python's convenience for non-critical components (I/O, networking, JSON parsing)
3. **Performance Balance**: Critical path (DFA execution) runs at native C speed while maintaining rapid development cycle
4. **Professional Architecture**: Mirrors real-world system design patterns (core library + high-level bindings)
5. **Testing Advantages**: Enables both unit testing (C) and integration testing (Python) independently

---

## II. PROJECT STRUCTURE AND ORGANIZATION

### A. Directory Hierarchy

```
project-root/
│
├── data/                          # Lexicon data files
│   ├── prefix_table.json
│   ├── suffix_table.json
│   ├── bisaya_roots.json
│   ├── tagalog_roots.json
│   └── shared_vocab.json
│
├── src/
│   ├── c/                         # C implementation (core logic)
│   │   ├── analyzer.c             # DFA state machine implementation
│   │   ├── analyzer.h             # Public API header
│   │   ├── dictionary.c           # Hash table implementation
│   │   ├── dictionary.h           # Dictionary data structures
│   │   ├── morphology.c           # Morphological validation rules
│   │   ├── morphology.h           # Morphology interface
│   │   └── utils.c                # String utilities
│   │
│   └── python/                    # Python wrapper layer
│       ├── __init__.py
│       ├── bindings.py            # ctypes C-to-Python interface
│       ├── loader.py              # JSON data loading utilities
│       ├── server.py              # Flask web server
│       └── detector.py            # Code-switching detection logic
│
├── build/                         # Compiled object files (generated)
│   └── *.o
│
├── lib/                           # Compiled shared library (generated)
│   └── libanalyzer.so             # Main C library
│
├── web/                           # Frontend assets
│   ├── templates/
│   │   └── index.html             # Main web interface
│   └── static/
│       ├── script.js              # Client-side JavaScript
│       └── style.css              # Styling
│
├── tests/                         # Testing suite
│   ├── test_c/                    # C unit tests
│   │   ├── test_main.c
│   │   ├── test_dfa.c
│   │   └── test_dictionary.c
│   └── test_python/               # Python integration tests
│       ├── test_bindings.py
│       ├── test_analyzer.py
│       └── test_detector.py
│
├── docs/                          # Documentation
│   ├── proposal.md                # Original project proposal
│   ├── api_reference.md           # C API documentation
│   └── user_guide.md              # Usage instructions
│
├── Makefile                       # Build automation
├── requirements.txt               # Python dependencies
├── .gitignore
└── README.md
```

### B. File Responsibilities Matrix

| Component | Language | Purpose | Lines (Est.) |
|-----------|----------|---------|--------------|
| analyzer.c | C | DFA state machine, main parsing logic | 300-400 |
| dictionary.c | C | Hash table implementation | 200-300 |
| morphology.c | C | Validation rules, compatibility checks | 150-200 |
| utils.c | C | String manipulation helpers | 100-150 |
| bindings.py | Python | C library wrapper, type conversions | 150-200 |
| loader.py | Python | JSON loading, data preprocessing | 100-150 |
| server.py | Python | Flask web server, API endpoints | 100-150 |
| detector.py | Python | Code-switching detection algorithm | 100-150 |

---

## III. DATA LAYER DESIGN

### A. Lexicon File Format

**Format Choice: JSON**
- **Rationale**: Human-readable, standard format, easy to edit and validate
- **Python Parsing**: Native `json` library support
- **C Consumption**: Python converts JSON to simple text format before passing to C

### B. Data File Specifications

#### 1. Prefix Table (prefix_table.json)
```
Structure:
{
  "prefix_string": {
    "language": "Bisaya" | "Tagalog" | "Both",
    "function": "actor_focus_completed" | "object_focus" | etc.,
    "compatible_pos": ["verb", "noun", "adjective"]
  }
}

Target Size: 30-40 entries
Example Entries: nag-, mag-, gi-, um-, pag-, ma-, ka-, mi-, ni-, maN-, paN-
```

#### 2. Suffix Table (suffix_table.json)
```
Structure:
{
  "suffix_string": {
    "language": "Bisaya" | "Tagalog" | "Both",
    "function": "locative_focus" | "object_focus" | etc.,
    "compatible_pos": ["verb", "noun"]
  }
}

Target Size: 30-40 entries
Example Entries: -an, -on, -in, -han, -a, -i, -ng
```

#### 3. Root Word Lexicons (bisaya_roots.json, tagalog_roots.json)
```
Structure:
{
  "root_word": {
    "meaning": "English translation",
    "pos": "verb" | "noun" | "adjective" | "adverb",
    "notes": "Optional usage notes"
  }
}

Target Size: 200+ entries each
Categories: Common verbs (eat, sleep, go), common nouns (house, water, food), 
            common adjectives (big, small, good)
```

#### 4. Shared Vocabulary (shared_vocab.json)
```
Structure:
{
  "shared_word": {
    "meaning": "English translation",
    "pos": "verb" | "noun" | "adjective",
    "origin": "Indigenous" | "Spanish loan" | "English loan"
  }
}

Target Size: 50+ entries
Focus: Cognates, loanwords appearing identically in both languages
Examples: luto, tubig, libro, eskwela, mesa, silya
```

### C. Data Loading Strategy

**Phase 1: Python Layer**
1. Read JSON files using Python's `json.load()`
2. Validate JSON structure and required fields
3. Convert to simplified format for C consumption
4. Options for C format:
   - **Option A**: Newline-delimited text (simplest)
   - **Option B**: Custom binary format (faster but complex)
   - **Recommended**: Option A for development, Option B if performance issues arise

**Phase 2: C Layer**
1. Receive preprocessed string data from Python
2. Parse line-by-line into hash table structures
3. Build in-memory hash tables optimized for O(1) lookup
4. Maintain separate hash tables for each lexicon type

### D. Data Collection and Population Plan

**Initial Implementation (Weeks 1-2):**
- Minimum viable dataset: 50 roots per language, 10 affixes each
- Purpose: Enable basic testing and DFA development
- Source: Manual entry from linguistic references

**Full Implementation (Weeks 3-4):**
- Complete dataset: 200+ roots per language, 30-40 affixes each
- Sources:
  - Cebuano-English online dictionaries (Binisaya.com)
  - TagDict for Tagalog
  - Linguistic papers (Roxas 2007, MAG-Tagalog references)
  - Native speaker consultation for validation
- Quality control: Cross-reference entries, validate with sample sentences

---

## IV. C CORE IMPLEMENTATION

### A. Hash Table Implementation (dictionary.c/h)

**Purpose**: Provide O(1) average-case lookup for all lexicons

**Data Structures:**
```
HashTable Structure:
- Array of buckets (linked list for collision resolution)
- Size: Prime number for better distribution (e.g., 251 for 200+ entries)
- Load factor: Target 0.7 (resize if exceeded)

Entry Structure:
- Key: Word/affix string
- Value: Metadata (language, function, POS, etc.)
- Next pointer: For collision chaining
```

**Functions to Implement:**
1. **ht_create(size)**: Allocate and initialize hash table
2. **ht_insert(table, key, value)**: Add entry with collision handling
3. **ht_lookup(table, key)**: Search and return value or NULL
4. **ht_exists(table, key)**: Boolean check for membership
5. **ht_destroy(table)**: Free all allocated memory
6. **hash_function(key)**: DJB2 or similar proven hash algorithm

**Hash Function Choice:**
- DJB2 algorithm (simple, effective for strings)
- Alternative: FNV-1a if better distribution needed
- Include unit tests for distribution quality

**Memory Management:**
- All strings allocated with malloc(), freed with free()
- Deep copy of keys and values (don't store pointers to external data)
- Cleanup function called at program termination

### B. DFA State Machine (analyzer.c/h)

**State Enumeration:**
```
typedef enum {
    STATE_START = 0,              // q0: Initial state, ready to parse
    STATE_PREFIX_STRIPPED = 1,    // q1: Prefix identified/skipped
    STATE_ROOT_VALIDATED = 2,     // q2: Valid root found in lexicon
    STATE_SUFFIX_STRIPPED = 3,    // q3: Suffix identified/skipped
    STATE_ACCEPT = 4,             // q4: Complete valid parse
    STATE_REJECT = 5              // q_reject: Invalid morphology
} State;
```

**Analysis Result Structure:**
```
typedef struct {
    bool valid;                   // Parse successful?
    char word[256];               // Original input word
    char prefix[64];              // Identified prefix (or empty)
    char root[128];               // Identified root word
    char suffix[64];              // Identified suffix (or empty)
    Language language;            // Root word's language
    char trace[10][256];          // State transition log (debugging)
    int trace_count;              // Number of trace entries
} AnalysisResult;
```

**Core Algorithm Flow:**

**Step 1: Prefix Detection (q0 → q1)**
- Input: Full word string
- Process:
  1. Iterate through all prefixes in prefix_table
  2. Check if word starts with each prefix (strncmp)
  3. If match found:
     - Store prefix in result
     - Remove prefix from remaining string
     - Log transition: "q0→q1: Found prefix 'X'"
  4. If no match:
     - Prefix remains NULL
     - Remaining = full word
     - Log transition: "q0→q1: No prefix detected"
- Output: Remaining string without prefix
- Next state: q1

**Step 2: Root Identification (q1 → q2 or q_reject)**
- Input: Remaining string after prefix removal
- Process:
  1. **First attempt**: Check if entire remaining string is a valid root
     - Search bisaya_roots hash table
     - Search tagalog_roots hash table
     - Search shared_vocab hash table
     - If found: Store root, language, proceed to Step 2c
  2. **Second attempt** (if Step 2a fails): Try suffix stripping
     - Iterate through all suffixes in suffix_table
     - Check if remaining string ends with each suffix (strcmp on substring)
     - If match found:
       - Extract potential root (remaining - suffix)
       - Check if potential root exists in any lexicon
       - If valid root found: Store root, suffix, language
  3. **Failure case**: If no valid root found in either attempt
     - Log: "q1→REJECT: No valid root"
     - Transition to q_reject
     - Return invalid result
- Output: Root word, language, optional suffix
- Next state: q2 (success) or q_reject (failure)

**Step 3: Suffix Handling (q2 → q3)**
- Input: Root identified, check for suffix
- Process:
  - If suffix was found during root identification (Step 2):
    - Already stored in result
    - Log: "q2→q3: Suffix 'X' identified"
  - If no suffix:
    - Suffix remains NULL
    - Log: "q2→q3: No suffix present"
- Output: Complete morpheme decomposition
- Next state: q3

**Step 4: Validation (q3 → q4 or q_reject)**
- Input: Complete morpheme set (prefix, root, suffix)
- Process: Validate morphological compatibility
  1. **Prefix-Root compatibility**:
     - Check if prefix's compatible_pos includes root's POS
     - Check if prefix's language matches root (or prefix is "Both")
  2. **Suffix-Root compatibility**:
     - Check if suffix's compatible_pos includes root's POS
     - Check if suffix's language matches root (or suffix is "Both")
  3. **Intra-word code-switching check**:
     - Reject if Bisaya prefix + Tagalog root (or vice versa)
     - Allow if any component is "Both" or "Shared"
- Output: Validation result
- Next state: q4 (valid) or q_reject (invalid)

**Step 5: Accept/Reject (Terminal States)**
- **q4 (ACCEPT)**:
  - Set result.valid = true
  - Log: "q3→q4: ACCEPT - Valid morphology"
  - Return complete AnalysisResult
- **q_reject (REJECT)**:
  - Set result.valid = false
  - Log reason for rejection
  - Return AnalysisResult with error information

### C. Public API Functions

**Primary Functions:**
```
void init_analyzer(const char* prefix_data, 
                   const char* suffix_data,
                   const char* bisaya_data, 
                   const char* tagalog_data,
                   const char* shared_data);
```
- Called once at program startup
- Receives preprocessed lexicon data from Python
- Initializes all hash tables
- Returns void (failure causes program exit with error message)

```
void analyze_word(const char* word, AnalysisResult* result);
```
- Main entry point for morphological analysis
- Input: UTF-8 encoded word string
- Output: Populated AnalysisResult structure (passed by reference)
- Thread-safety: Not guaranteed (single-threaded use assumed)

```
void cleanup_analyzer();
```
- Called once at program termination
- Frees all hash table memory
- Ensures clean shutdown (important for valgrind testing)

**Helper Functions:**
```
const char* language_to_string(Language lang);
```
- Converts Language enum to human-readable string
- Used for result serialization

```
char* find_prefix(HashTable* table, const char* word);
```
- Internal helper: searches for matching prefix
- Returns prefix string or NULL

```
char* find_suffix(HashTable* table, const char* word);
```
- Internal helper: searches for matching suffix
- Returns suffix string or NULL

### D. Morphological Validation Rules (morphology.c/h)

**Compatibility Matrix:**
Build validation logic based on linguistic rules:

1. **Prefix-POS Compatibility**:
   - Actor focus prefixes (nag-, mag-): Only verbs
   - Nominalizers (pag-, ka-): Only verbs (converts to noun)
   - Stative prefixes (ma-): Verbs and adjectives

2. **Suffix-POS Compatibility**:
   - Object focus (-in, -on): Only verbs
   - Locative focus (-an, -han): Only verbs
   - Adjective markers: Only roots that can become adjectives

3. **Language Consistency**:
   - Bisaya-specific affixes (gi-, -on): Must pair with Bisaya or Shared roots
   - Tagalog-specific affixes (um-, -in): Must pair with Tagalog or Shared roots
   - "Both" affixes: Can pair with any root language

**Implementation Strategy**:
- Create lookup tables for compatibility rules
- Implement validation functions that check rule compliance
- Return detailed error messages for debugging

### E. String Utilities (utils.c)

**Required Functions:**
1. **safe_strcpy(dest, src, size)**: Bounds-checked string copy
2. **starts_with(str, prefix)**: Check prefix match
3. **ends_with(str, suffix)**: Check suffix match
4. **strip_prefix(word, prefix)**: Remove prefix, return remainder
5. **strip_suffix(word, suffix)**: Remove suffix, return remainder

**UTF-8 Handling**:
- Most operations work byte-wise (prefixes/suffixes are ASCII in this project)
- If special characters needed (ñ, á, etc.): Use UTF-8 aware string functions
- Consider: Is full UTF-8 support necessary for this dataset? (Probably not for minimal implementation)

### F. Memory Management Strategy

**Allocation Points:**
1. Hash table buckets (malloc during ht_create)
2. Hash table entries (malloc during ht_insert)
3. String duplication (strdup for keys and values)

**Deallocation Requirements:**
1. Free all hash table entries (iterate through buckets)
2. Free all strings (keys and values)
3. Free bucket arrays
4. Free hash table structures

**Testing for Leaks:**
- Use valgrind: `valgrind --leak-check=full ./test_program`
- Target: 0 bytes leaked, 0 errors
- Document any suppressions needed

### G. Compilation and Build

**Compiler Flags:**
- `-Wall -Wextra`: Enable all warnings
- `-Werror`: Treat warnings as errors (strict mode)
- `-fPIC`: Position-independent code (required for shared library)
- `-O2`: Optimization level 2 (balance speed/debug)
- `-g`: Include debugging symbols (for gdb/valgrind)

**Build Targets:**
1. **Object files**: Compile each .c file to .o
2. **Shared library**: Link all .o files into libanalyzer.so
3. **Test executables**: Link test files with library
4. **Clean target**: Remove all generated files

**Makefile Organization:**
- Variables for compiler, flags, directories
- Pattern rules for .c → .o compilation
- Explicit rules for library creation
- Dependency tracking (recompile on header changes)
- Phony targets: all, clean, test, install

---

## V. PYTHON WRAPPER LAYER

### A. Foreign Function Interface (bindings.py)

**Purpose**: Bridge between Python and C, handle type conversions

**ctypes Setup:**
```
Steps:
1. Load compiled shared library using ctypes.CDLL()
2. Define C structure equivalents as ctypes.Structure classes
3. Specify function signatures (argtypes, restype) for all C functions
4. Create Python wrapper classes that hide ctypes complexity
```

**Structure Mapping:**
- C `AnalysisResult` struct → Python `AnalysisResult` class (inherits ctypes.Structure)
- Define `_fields_` list matching C struct exactly
- Handle array types: `(ctypes.c_char * 256) * 10` for trace array

**Function Signature Declaration:**
For each C function:
1. Specify argument types: `function.argtypes = [ctypes.c_char_p, ...]`
2. Specify return type: `function.restype = ctypes.c_void_p` (or appropriate type)
3. Document: What C function does, how Python calls it

**Type Conversion Strategy:**

**Python → C:**
- Python str → bytes (UTF-8 encoding): `word.encode('utf-8')`
- Python dict → C char*: Convert to JSON or delimited string
- Python list → C array: Use ctypes array types

**C → Python:**
- C char* → Python str: `.decode('utf-8')`
- C struct → Python dict: Manual field extraction
- C arrays → Python lists: Iterate and convert each element

**Error Handling:**
- Catch ctypes exceptions (OSError if library not found)
- Validate C return values (NULL pointer checks)
- Provide informative Python exceptions

### B. Morphological Analyzer Class (bindings.py)

**Class Design:**
```
class MorphologicalAnalyzer:
    Purpose: High-level Python interface to C analyzer
    
    Responsibilities:
    - Load JSON lexicon files
    - Convert JSON to C-compatible format
    - Initialize C library
    - Provide analyze() method for single words
    - Handle cleanup on deletion
```

**Initialization Process:**
1. Locate data directory (configurable path)
2. Load all five JSON files using Python's json module
3. Validate JSON structure (required fields present)
4. Convert JSON dictionaries to format C expects:
   - Extract just keys for root word lists
   - Format as newline-delimited strings
   - Encode as UTF-8 bytes
5. Call C `init_analyzer()` with processed data
6. Store library reference for later use

**Analysis Method:**
```
def analyze(self, word: str) -> dict:
    Purpose: Analyze single word using C DFA
    
    Process:
    1. Create AnalysisResult instance (ctypes structure)
    2. Encode input word to UTF-8 bytes
    3. Call C analyze_word() with pointer to result struct
    4. Extract fields from C result struct
    5. Convert C types to Python types
    6. Build Python dictionary with results
    7. Return dictionary
    
    Return format:
    {
        "valid": bool,
        "word": str,
        "prefix": str or None,
        "root": str or None,
        "suffix": str or None,
        "language": str ("Bisaya", "Tagalog", "Shared", "Unknown"),
        "trace": list of str (state transitions)
    }
```

**Resource Management:**
- Implement `__del__` method to call C cleanup_analyzer()
- Use context manager protocol if needed: `__enter__`, `__exit__`
- Document: Users should create one analyzer instance, reuse it

### C. Data Loader Module (loader.py)

**Purpose**: Centralize JSON loading and validation logic

**Functions:**

```
load_lexicon(filepath: str) -> dict:
    - Open and parse JSON file
    - Validate structure (required fields present)
    - Return dictionary
    - Raise exception on errors (file not found, invalid JSON, schema mismatch)
```

```
validate_prefix_table(data: dict) -> bool:
    - Check each entry has: language, function, compatible_pos
    - Validate language values: "Bisaya", "Tagalog", or "Both"
    - Return True if valid, raise exception if not
```

```
validate_root_lexicon(data: dict) -> bool:
    - Check each entry has: meaning, pos
    - Validate POS values: "verb", "noun", "adjective", "adverb"
    - Return True if valid, raise exception if not
```

```
convert_to_c_format(data: dict, format_type: str) -> bytes:
    - Convert Python dict to format C expects
    - For root lists: Extract keys, join with newlines
    - For affix tables: May need to include metadata
    - Return UTF-8 encoded bytes
```

**Error Handling Strategy:**
- Custom exception classes: `LexiconLoadError`, `ValidationError`
- Detailed error messages (which file, which field, what's wrong)
- Fail fast: Don't initialize if data is invalid

### D. Flask Web Server (server.py)

**Purpose**: Provide HTTP API and serve web interface

**Flask Application Setup:**
```
Configuration:
- template_folder: Points to web/templates/
- static_folder: Points to web/static/
- Debug mode: Enabled for development, disabled for production
- Port: 8000 (configurable)
- Host: localhost (or 0.0.0.0 for network access)
```

**Route Definitions:**

**Route 1: Homepage**
```
@app.route('/')
Method: GET
Purpose: Serve main web interface
Response: Render index.html template
```

**Route 2: Analysis API**
```
@app.route('/api/analyze')
Method: POST
Content-Type: application/json

Request body:
{
    "text": "Nag-luto siya ug pag-kaon sa kusina"
}

Process:
1. Extract text from JSON request
2. Tokenize text into words (split on whitespace)
3. For each word:
   - Call analyzer.analyze(word)
   - Collect result
4. Call code-switching detector with results
5. Build response JSON with word analyses and switch points

Response body:
{
    "words": [
        {
            "valid": true,
            "word": "Nag-luto",
            "prefix": "Nag-",
            "root": "luto",
            "suffix": null,
            "language": "Shared",
            "trace": [...]
        },
        ...
    ],
    "switches": [
        {
            "position": 3,
            "from": "Tagalog",
            "to": "Bisaya"
        }
    ],
    "summary": {
        "total_words": 6,
        "valid_words": 6,
        "invalid_words": 0,
        "switch_count": 1
    }
}
```

**Error Handling:**
- 400 Bad Request: Invalid JSON, missing fields
- 500 Internal Server Error: C library errors, unexpected exceptions
- Include error messages in JSON response
- Log errors server-side for debugging

**CORS Configuration** (if needed for development):
- Enable CORS headers for local testing
- Restrict in production

### E. Code-Switching Detection Module (detector.py)

**Purpose**: Identify language transition points in analyzed text

**Main Function:**
```
def detect_switches(word_analyses: list) -> list:
    Purpose: Find code-switching boundaries
    
    Algorithm:
    1. Initialize: previous_language = None, switches = []
    2. For each word analysis in sequence:
        a. Get current_language from analysis
        b. Skip if language is "Shared" or "Unknown" (don't count as distinct)
        c. If previous_language exists and differs from current_language:
           - Record switch point: position, from_language, to_language
        d. Update previous_language (skip if "Shared"/"Unknown")
    3. Return list of switch points
    
    Return format:
    [
        {
            "position": 3,          # Word index where switch occurs
            "from": "Tagalog",      # Previous language
            "to": "Bisaya",         # New language
            "context": {            # Optional: surrounding words
                "before": "word",
                "after": "word"
            }
        },
        ...
    ]
```

**Helper Functions:**

```
def get_language_sequence(word_analyses: list) -> list:
    - Extract just the language tags from analyses
    - Return list: ["Tagalog", "Tagalog", "Bisaya", ...]
    - Useful for visualization
```

```
def calculate_switch_frequency(switches: list, total_words: int) -> float:
    - Metric: switches per 100 words
    - Formula: (len(switches) / total_words) * 100
    - Return float
```

```
def identify_matrix_language(word_analyses: list) -> str:
    - Determine dominant language (most frequent)
    - Count occurrences of each language (excluding "Shared")
    - Return "Bisaya", "Tagalog", or "Balanced"
```

**Edge Cases to Handle:**
1. All words are "Shared" → No switches detected
2. Single word input → Cannot have switches
3. Alternating every word → High switch frequency
4. Invalid words in sequence → Skip them in switch detection

---

## VI. WEB INTERFACE DESIGN

### A. Frontend Architecture

**Technology Stack:**
- HTML5: Structure
- CSS3: Styling (optional: Tailwind CSS or Bootstrap)
- Vanilla JavaScript: Logic (no frameworks needed for simplicity)
- Fetch API: Communicate with Flask backend

### B. HTML Structure (index.html)

**Page Layout:**
```
Components:
1. Header Section:
   - Project title
   - Brief description
   - Optional: University logo

2. Input Section:
   - Large text area for input (minimum 5 rows)
   - Placeholder text: "Enter Bisaya-Tagalog mixed text..."
   - Character counter (optional)
   - "Analyze" button (primary action)
   - "Clear" button (secondary action)

3. Results Section (initially hidden):
   - Word-by-word analysis table
   - Code-switching visualization
   - Summary statistics
   - DFA trace logs (collapsible/expandable)

4. Footer Section:
   - Team members
   - Academic info (course, university, date)
   - Optional: Link to documentation
```

**Results Table Structure:**
```
Columns:
1. # (Word number)
2. Word (original form)
3. Prefix (or "—" if none)
4. Root (highlighted)
5. Suffix (or "—" if none)
6. Language (color-coded badge)
7. Valid? (✓ or ✗)

Features:
- Sortable columns (optional)
- Hover tooltips with additional info
- Color coding by language
```

### C. JavaScript Logic (script.js)

**Main Functions:**

```
async function analyzeText():
    Purpose: Handle analyze button click
    
    Process:
    1. Get text from textarea
    2. Validate: not empty, reasonable length
    3. Show loading indicator
    4. Make POST request to /api/analyze
    5. Wait for response
    6. Hide loading indicator
    7. If success: Call displayResults()
    8. If error: Call displayError()
```

```
function displayResults(data):
    Purpose: Render analysis results in UI
    
    Process:
    1. Clear previous results
    2. Show results section
    3. Build word analysis table:
       - Create table rows from data.words
       - Apply color coding by language
       - Add valid/invalid indicators
    4. Render code-switching visualization:
       - Timeline or arrow diagram
       - Highlight switch points
    5. Display summary statistics:
       - Total words, valid words, switch count
    6. Optional: Render DFA traces (accordion UI)
```

```
function visualizeSwitches(switches, words):
    Purpose: Create visual representation of language transitions
    
    Options:
    - Timeline: Horizontal bar showing language segments
    - Arrows: Language labels with arrows between switches
    - Color blocks: Each word colored by language
    
    Recommended: Color blocks (simplest to implement)
```

```
function displayError(error):
    Purpose: Show error message to user
    
    Process:
    1. Display error notification (red banner or modal)
    2. Show error message from server or generic message
    3. Log detailed error to console for debugging
    4. Provide suggestions (check input, try again)
```

**Event Listeners:**
- Analyze button: onClick → analyzeText()
- Clear button: onClick → clearInput(), clearResults()
- Text area: onInput → updateCharacterCount() (optional)
- Collapsible sections: onClick → toggleExpanded()

**API Communication:**
```
Fetch configuration:
- Method: POST
- Headers: Content-Type: application/json
- Body: JSON.stringify({text: inputText})
- Error handling: try-catch for network errors
- Timeout: Optional, 10 seconds max
```

### D. CSS Styling (style.css)

**Design Principles:**
1. Clean, academic appearance
2. Responsive layout (mobile-friendly)
3. Accessible (WCAG AA compliance)
4. Color-blind friendly palette

**Color Scheme:**
```
Language color coding:
- Bisaya: Blue (#3B82F6)
- Tagalog: Green (#10B981)
- Shared: Gray (#6B7280)
- Unknown/Invalid: Red (#EF4444)

UI colors:
- Background: White or light gray
- Text: Dark gray (not pure black)
- Borders: Light gray
- Accents: University colors (optional)
```

**Responsive Breakpoints:**
- Desktop: 1024px and up (full layout)
- Tablet: 768px to 1023px (adjusted columns)
- Mobile: below 768px (stacked layout)

**Key Styles:**
- Text area: Monospace font for better readability
- Table: Alternating row colors, hover effects
- Buttons: Clear visual states (default, hover, active, disabled)
- Loading indicator: Spinner or progress bar
- Error messages: Prominent but not alarming

### E. User Experience Flow

**Typical User Journey:**
1. User loads page
2. Sees input area with example text
3. Types or pastes Bisaya-Tagalog text
4. Clicks "Analyze"
5. Sees loading indicator (brief)
6. Results appear smoothly (no jarring transitions)
7. Reviews word-by-word breakdown
8. Examines code-switching points
9. Optionally views DFA traces for learning
10. Can analyze new text (clear and repeat)

**Performance Expectations:**
- Page load: < 1 second
- Analysis response: < 500ms for 50-word sentence
- UI updates: Smooth, no lag
- Progressive enhancement: Core functionality works without JS

---

## VII. TESTING STRATEGY

### A. C Unit Testing

**Testing Framework Options:**
1. **Custom test harness** (simplest, no dependencies)
2. **Check framework** (standard C testing library)
3. **Unity** (lightweight, designed for embedded systems)

**Recommended: Custom harness for simplicity**

**Test Structure (test_main.c):**
```
Organization:
- main() function that runs all tests
- Each test function returns bool (pass/fail)
- Print results: "PASS: test_name" or "FAIL: test_name (reason)"
- Final summary: X/Y tests passed
- Exit code: 0 if all pass, 1 if any fail
```

**Test Categories:**

**1. Hash Table Tests (test_dictionary.c):**
```
Test cases:
- test_create_destroy: Create table, verify non-NULL, destroy
- test_insert_single: Insert one entry, verify successful
- test_lookup_existing: Insert entry, lookup should return value
- test_lookup_nonexistent: Lookup missing key should return NULL
- test_insert_duplicate: Insert same key twice, verify handling
- test_collision_handling: Insert keys that hash to same bucket
- test_large_dataset: Insert 500 entries, verify all retrievable
- test_memory_cleanup: Run with valgrind, verify no leaks
```

**2. DFA Logic Tests (test_dfa.c):**
```
Test cases:
- test_simple_root: Word with no affixes ("kaon" → root only)
- test_prefix_only: Word with prefix ("nag-luto" → prefix + root)
- test_suffix_only: Word with suffix ("kaon-an" → root + suffix)
- test_prefix_and_suffix: Word with both ("pag-kaon-an" → all three)
- test_unknown_word: Invalid word → should reject
- test_bisaya_word: Verify Bisaya detection
- test_tagalog_word: Verify Tagalog detection
- test_shared_word: Verify Shared detection
- test_mixed_affixes: Invalid combination → should reject
- test_empty_string: Handle edge case gracefully
- test_very_long_word: Test buffer limits
```

**3. Morphology Validation Tests (test_morphology.c):**
```
Test cases:
- test_valid_combinations: Known good prefix+root+suffix
- test_invalid_prefix_pos: Prefix incompatible with root POS
- test_invalid_suffix_pos: Suffix incompatible with root POS
- test_language_mismatch: Bisaya affix + Tagalog root
- test_both_compatibility: "Both" affixes work with both languages
```

**4. String Utility Tests (test_utils.c):**
```
Test cases:
- test_starts_with: Various prefix scenarios
- test_ends_with: Various suffix scenarios
- test_strip_prefix: Correct substring extraction
- test_strip_suffix: Correct substring extraction
- test_unicode_handling: If applicable, test non-ASCII characters
```

**Test Data Files:**
- Create small test lexicons (10-20 entries each)
- Known good cases: Words that should parse correctly
- Known bad cases: Words that should be rejected
- Edge cases: Empty strings, very long strings, special characters

**Automation:**
```
make test:
    1. Compile test suite with library
    2. Run test executable
    3. Capture output
    4. Check exit code
    5. Report results
```

### B. Python Integration Testing

**Testing Framework: pytest**

**Test Structure (test_python/):**
```
test_bindings.py:
    Purpose: Test C-Python interface
    
    Tests:
    - test_library_loads: Verify .so file found and loaded
    - test_init_analyzer: Verify initialization succeeds
    - test_analyze_returns_dict: Verify return format
    - test_type_conversions: Python str → C → Python correct
    - test_unicode_handling: Non-ASCII characters handled
    - test_cleanup: Verify cleanup doesn't crash
```

```
test_analyzer.py:
    Purpose: Test end-to-end analysis
    
    Tests:
    - test_bisaya_words: 10 known Bisaya words → correct parse
    - test_tagalog_words: 10 known Tagalog words → correct parse
    - test_shared_words: 5 shared words → correct detection
    - test_invalid_words: Words not in lexicon → rejected
    - test_complex_morphology: Multi-affix words
    - test_batch_processing: Analyze 100 words → all succeed
```

```
test_detector.py:
    Purpose: Test code-switching detection
    
    Tests:
    - test_no_switches: Monolingual sentence → 0 switches
    - test_single_switch: One transition → 1 switch detected
    - test_multiple_switches: Alternating → all switches found
    - test_shared_words_ignored: "Shared" doesn't trigger switch
    - test_switch_positions: Verify correct word indices
    - test_matrix_language: Verify dominant language calculation
```

```
test_server.py:
    Purpose: Test Flask API (optional, use Flask test client)
    
    Tests:
    - test_homepage_loads: GET / returns 200
    - test_analyze_endpoint: POST /api/analyze returns JSON
    - test_invalid_json: POST malformed JSON → 400 error
    - test_empty_text: POST empty text → handled gracefully
    - test_response_format: Verify JSON structure correct
```

**Test Execution:**
```
Commands:
- pytest tests/test_python/                    # Run all tests
- pytest tests/test_python/test_analyzer.py    # Run specific file
- pytest -v                                     # Verbose output
- pytest --cov=src/python                      # Code coverage
```

