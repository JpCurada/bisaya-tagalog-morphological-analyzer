# Bisaya-Tagalog Morphological Analyzer

A hybrid C/Python system that performs morphological analysis and code-switching detection on mixed Bisaya-Tagalog text using Deterministic Finite Automata (DFA).

## Project Structure

- `data/`: Lexicon data files (JSON)
- `src/c/`: C implementation of DFA and hash tables
- `src/python/`: Python wrapper, web server, and utilities
- `web/`: Frontend assets
- `tests/`: Unit and integration tests

## Prerequisites

- GCC Compiler
- Python 3.x
- Make

## Build and Run

1. **Build the C library:**
   ```bash
   make
   ```

2. **Install Python dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

3. **Run the Web Server:**
   ```bash
   python src/python/server.py
   ```

## Usage

Access the web interface at `http://localhost:8000`. Enter mixed Bisaya-Tagalog text to analyze morphology and detect code-switching.
