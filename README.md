# Bisaya-Tagalog Morphological Analyzer

A finite-state morphological analyzer that performs morphological decomposition and code-switching detection on mixed Bisaya-Tagalog text using a python-based Non-Deterministic Finite Automaton (NFA).

## Project Structure

- `data_v2/`: Lexicon data files (JSON) - Prefix, Infix, Suffix, Circumfix tables, and Root Lexicons.
- `src/python/`: Core logic including FSM implementation (`bindings.py`) and Web Server (`server.py`).
- `web/`: Frontend assets (HTML, CSS, JS).
- `docs/`: Formal documentation and academic proposal.

## Prerequisites

- Python 3.x

## Build and Run

1. **Install Python dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

2. **Run the Web Server:**
   ```bash
   python src/python/server.py
   ```

## Usage

Access the web interface at `http://localhost:8000`. Enter mixed Bisaya-Tagalog text to analyze morphology and detect code-switching.

## System Architecture

The analyzer helps linguistic research by decomposing words using a **Finite-State Morphotactic approach**. The system consists of four interacting components controlled by a global automaton:

1.  **PrefixFSM**: Lexical automaton for prefix tokens.
2.  **InfixFSM**: Handling ε-transitions for inserting morphemes.
3.  **RootLexicon**: Stem lexicon lookup structure for validation.
4.  **SuffixCircumfixFSM**: Lexical automaton for suffixes and circumfix constraints.

## Finite-State Morphotactic Output Format

The system outputs a formal morphotactic parse string where morphemes are separated with `+` and annotated with tags derived from the FSM states.

**Examples:**
*   `magsulat` -> `mag[PFX] + sulat[ROOT]`
*   `sinulat` -> `in[INFX] + sulat[ROOT]`
*   `kasulatan` -> `ka[CIRCUMFIX_PREFIX] + sulat[ROOT] + an[CIRCUMFIX_SUFFIX]`
