In Partial Fulfillment of the Requirements for Automata and Language Theory

# **Bisaya-Tagalog Code-Switching Morphological Analyzer Using Finite Automata**

**Group 1**  
**Arabia, Clark Michael D.**  
**Brosas, Joshua**  
**Curada, John Paul M.**  
**Zaragoza, Marie Criz P.**

DECEMBER 2025

## **I. INTRODUCTION AND BACKGROUND**

Language technology for regional Philippine languages remains underdeveloped compared to major languages such as English or Tagalog. Code-switching between Bisaya (Cebuano) and Tagalog is a prevalent linguistic phenomenon in the Philippines, particularly in regions where both languages are commonly spoken. This bilingual practice presents unique challenges for natural language processing (NLP) systems, as traditional morphological analyzers are typically designed for monolingual contexts.

A morphological analyzer functions by decomposing words into their constituent morphemes (prefix, root, suffix) and identifying their linguistic properties. In theoretical terms, this process can be modeled using **Finite Automata with integrated lookup tables**, where each valid morphological structure corresponds to an accepted path through a state machine with dictionary validation at specific states.

Today, users who work with mixed Bisaya-Tagalog text lack computational tools that can systematically analyze morphological structures across both languages. Existing NLP tools either process one language exclusively or fail to recognize code-switching boundaries. This makes linguistic research, language documentation, and computational analysis of bilingual Philippine text inconvenient and error-prone.

This project aims to construct a **lexicon-based morphological analyzer using a Non-Deterministic Finite Automaton (NFA) approach via Breadth-First Search (BFS)**. Unlike strict Deterministic Finite Automata (DFA) which are limited to single-path execution, our architecture allows for **parallel exploration of multiple valid morphological interpretations**, essential for handling the high degree of ambiguity in Philippine languages (e.g., overlapping affixes and morphophonemic changes). It decomposes Bisaya-Tagalog code-switched words into morphological components, identifies the language of each token, and detects code-switching points. By grounding the project in automata theory, it directly demonstrates how formal language concepts can be applied to practical morphological analysis systems for complex, low-resource bilingual contexts.

## **II. RELATED WORKS**

Morphological analysis systems have become an essential component of natural language processing tools across various languages. Morphological analyzers commonly rely on foundational concepts from automata theory, particularly the use of **Finite State Automata (FSA)** for morphological decomposition and **lookup tables** for lexical validation. These methods have been extensively studied and applied across various languages; however, bilingual morphological analysis for Philippine languages, particularly code-switching contexts, remains largely unexplored.

**Beesley and Karttunen (2003)** pioneered finite-state morphological analysis using Finite-State Transducers (FST). Their XEROX morphological analyzer demonstrated that complex morphological processes can be modeled as cascades of finite-state operations. They showed that affix stripping, root lookup, and morphological rule application can be unified under finite-state framework with lookup table integration. Their work achieved 94% accuracy on English morphological analysis and established the theoretical foundation for FST-based morphology.

Several studies highlight the role of **finite automata in efficient morphological recognition**. For example, **tries and DFA-based lexicons** provide deterministic lookup processes that operate in linear time relative to word length, making them optimal for real-time morphological analysis applications (Mohri, 2020). However, for languages with high ambiguity, **Non-Deterministic (NFA)** approaches or probabalistic models are often favoured to capture multiple valid readings.

**Lindén et al. (2009)** extended finite-state approaches to agglutinative languages with rich morphology, developing HFST (Helsinki Finite-State Technology) toolkit. They demonstrated that highly inflected languages like Finnish and Turkish can be analyzed using weighted finite-state transducers with lexicon lookup at accepting states. Their key contribution was showing how multiple morphological processes (affixation, vowel harmony, consonant gradation) can be composed into single efficient automaton with integrated lexical databases.

**Cheng et al. (2020)** applied finite automata to code-switching detection in Chinese-English bilingual text, using DFA states to represent language contexts and transition tables to capture switching patterns. They achieved 89% accuracy in language identification using character-level automata with language-specific character class lookup tables. Their work demonstrated that code-switching points can be reliably detected using finite-state models with appropriate feature extraction.

**Roxas (2007)** developed morphological analyzer for Tagalog using affix-stripping algorithm with root word dictionary lookup. While not explicitly using automata, their approach followed finite-state principles: systematic left-to-right affix removal with validation against lexical database. They catalogued 158 Tagalog affixes and achieved 82% accuracy on morphological decomposition. However, their system was monolingual and could not handle code-switching.

Despite these advancements, **no publicly available Bisaya-Tagalog bilingual morphological analyzer currently exists**, highlighting a clear gap in digital language support. Existing morphological analyzers for Philippine languages process single languages only and do not include code-switching detection capabilities. This lack of representation in computational tools creates difficulties for researchers and language workers who need to analyze mixed Bisaya-Tagalog text computationally.

## **III. OBJECTIVES OF THE PROJECT**

### A. General Objective

This project will utilize a **Non-Deterministic Finite Automaton (NFA) simulation using Breadth-First Search (BFS)** to decompose Bisaya-Tagalog code-switched words into morphological components (prefix, root, suffix). The system will strictly separate dictionary lookups for Bisaya and Tagalog to accurately identify the source language of each token and detect code-switching boundaries in mixed-language text.

### B. Specific Objectives

1.  **To develop refined bilingual lexicon lookup tables** containing:
    *   Bisaya root words (minimum 200 entries)
    *   Tagalog root words (minimum 200 entries)
    *   Bisaya affixes in prefix, infix, suffix, and circumfix tables
    *   Tagalog affixes in prefix, infix, suffix, and circumfix tables

2.  **To construct an NFA-based engine for morphological parsing** where:
    *   **States** represent parsing configuration (Current String, Parsing Stage, Collected Affixes)
    *   **Transitions** represent the stripping of a matching affix (splitting into multiple parallel states for overlapping affixes)
    *   **Search Strategy** utilizes a Queue-based Breadth-First Search to explore all potential analyses simultaneously
    *   **Accepting states** validate complete morphological decomposition against root dictionaries

3.  **To implement a robust affix-stripping algorithm** capable of:
    *   Handling **Ambiguity**: Processing words where multiple affix combinations are valid (e.g., *binata* as root vs. *bata* + *-in-*)
    *   Handling **Overlaps**: Distinguishing between overlapping prefixes (e.g., *ma-* vs *mag-*) using non-deterministic branching
    *   Handling **Circumfixes**: Identifying discontinuous morphemes (prefix + suffix pairs)

4.  **To develop a language identification module** that:
    *   Tags each word as Bisaya, Tagalog, or Both by querying independent root dictionaries
    *   Identifies code-switching points (transitions between languages)
    *   Generates language sequence annotations

5.  **To create a transition table representation** showing:
    *   State transitions based on morphological patterns
    *   Lookup table queries at each state
    *   Decision logic for accepting/rejecting word forms

6.  **To evaluate system performance** on:
    *   Morphological decomposition accuracy (target: ≥85%)
    *   Language identification accuracy (target: ≥90%)
    *   Code-switching detection precision (target: ≥80%)

## IV. SCOPE AND LIMITATIONS OF THE PROJECT

### Scope

The project focuses on developing a morphological analyzer that performs word-level Bisaya-Tagalog morphological decomposition using **Finite State Automata** principles. It employs a **Breadth-First Search (BFS)** algorithm to simulate Nondeterminism, allowing it to capture all valid morphological interpretations of a word.

**Linguistic Coverage:**

*   Bisaya (Cebuano) morphology: common affixes (pag-, nag-, mag-, -an, -on, -in, gi-, ma-, maN-)
*   Tagalog morphology: common affixes (mag-, nag-, um-, -an, -in, -han, pag-, paN-)
*   Root word lexicons: 200+ words per language covering high-frequency vocabulary
*   **Ambiguous/Bridge words**: Words existing in both dictionaries (e.g., "inom", "lakad") are identified as "Both".

**Morphological Processes Covered:**

*   Prefixation: single prefix attachment (pag-kaon, mag-luto)
*   Infixation: internal affix attachment (-um-, -in-)
*   Suffixation: single suffix attachment (kaon-an, luto-han)
*   Circumfixation: prefix + suffix combinations (pag-kaon-an, mag-luto-han)
*   Root word identification: validation against bilingual lexicons

**Automata Architecture:**

*   **NFA Simulation**: The system uses a dynamic state-queue to manage multiple active parsing paths.
*   **Parallel Search**: Capable of outputting multiple valid analysis results for a single ambiguous word.
*   **Dual-Dictionary Lookup**: Independent querying of Bisaya and Tagalog JSON datasets to ensure accurate language tagging.

### Limitations

However, the system is limited to identifying morphological structures at the word level and **does not handle grammar, syntax, or semantic analysis**. Its accuracy depends heavily on the completeness and quality of the Bisaya-Tagalog lexicon dataset, and it may not recognize:

*   **Deep Morphophonemic changes**: complex allomorphic variations beyond basic nasal assimilation (paN-)
*   **Full Reduplication**: forms like "kaon-kaon" (partial reduplication is supported)
*   **Multiple nested affixation**: highly complex agglutinations with >3 affixes may be limited by search depth
*   **Intra-word code-switching**: ambiguous cases like "mag-uli" (Tagalog prefix + Bisaya root) excluded
*   **Proper nouns, mixed-language expressions, informal slang**, or newly emerging terms commonly used in online communication
*   **Dialectal variations**: focuses on standard Bisaya (Cebuano) and Manila Tagalog only

## **V. SYSTEM DESIGN AND METHODOLOGY**

### System Architecture

![][image2]

### NFA-Based Morphological Recognition (Parallel Search)

**Figure 1. NFA State Exploration for Morphological Parsing**

![][image3]
*This illustrates how Bisaya-Tagalog words such as "Nag-kaon" are parsed. Unlike a linear DFA, the system forks into multiple paths when it encounters ambiguity (e.g., is "mag-" a single prefix or "ma-" + "g-"?), validating all paths against the dictionary.*

### Lookup Table Structures

**Table 1: Sample Prefix Lookup Table**

| Prefix | Language | Function | Example |
| :---- | :---- | :---- | :---- |
| mag- | Tagalog | Actor focus | mag-luto |
| nag- | Both | Actor focus (completed) | nag-luto, nag-kaon |
| pag- | Both | Nominalization | pag-kaon, pag-kain |
| gi- | Bisaya | Object focus | gi-kaon |
| um- | Tagalog | Actor focus | um-alis |
| ma- | Both | Ability/State | ma-kaon, ma-kain |

*Target: 30-40 prefix entries covering both languages*

**Table 2: Sample Suffix Lookup Table**

| Suffix | Language | Function | Example |
| :---- | :---- | :---- | :---- |
| -an | Both | Locative focus | kaon-an, kain-an |
| -on | Bisaya | Object focus | kaon-on |
| -in | Tagalog | Object focus | kain-in |
| -han | Tagalog | Locative/Benefactive | luto-han |
| -a | Bisaya | Imperative | kaon-a |

*Target: 30-40 suffix entries covering both languages*

**Table 3: Sample Root Lexicons**

| Bisaya Roots | Tagalog Roots |
| :---- | :---- |
| kaon (eat) | kain (eat) |
| balay (house) | bahay (house) |
| dako (big) | malaki (big) |
| gamay (small) | maliit (small) |
| ayo (good) | mabuti (good) |

*Target: 200+ Bisaya, 200+ Tagalog entries*

### Data Collection and Processing

**Data Collection:**

*   Gather Bisaya words from publicly available digital dictionaries (Cebuano-English dictionaries), linguistic archives, and academic sources
*   Gather Tagalog words from TagDict, Filipino linguistic resources, and lexicographic databases
*   Collect affix inventories from linguistic descriptions of both languages

**Data Processing:**

*   Clean and format collected words into structured lexicon files (JSON format)
*   Organize into separate tables: prefix_table.json, suffix_table.json, infix_table.json, circumfix_table.json, bisaya_roots.json, tagalog_roots.json
*   Implement efficient hash-based/set-based lookup for O(1) performance
*   Validate lexicon entries for consistency and accuracy

### Automata Construction

**Build an NFA simulation where:**

*   **Logic**: Uses a Breadth-First Search (BFS) Queue to explore states.
*   **States**: Each state captures `(CurrentSubstring, Stage, AccumulatedAffixes)`.
*   **Transitions**: Stripping an affix creates a *new* state in the queue.
*   **Ambiguity Handling**: If a word matches multiple prefixes (e.g., `ma-` and `mag-`), *two* new states are enqueued, ensuring both possibilities are explored.

**Algorithmic Concept:**

1.  **Start State**: `InputWord`
2.  **Strip Prefixes**: Check all matching prefixes. For each match, create a state `(RemainingString, PrefixStripped)`.
3.  **Strip Infixes**: Check for internal matches. Create states.
4.  **Strip Suffixes**: Check for trailing matches. Create states.
5.  **Root Check (Validation)**: At any point, check if `RemainingString` is in `BisayaDict` or `TagalogDict`.
    *   If **Yes**: Mark as **Accepted Analysis**. Record path.
    *   If **No**: Continue stripping or Discard if no moves left.

**Formal NFA Definition:**

NFA_Morph = (Q, Σ, Δ, q0, F)

*   **Q**: Set of all possible parsing states (infinite/dynamic in practice, bounded by word length)
*   **Σ**: Character alphabet + Affix IDs
*   **Δ (Transition Relation)**: Maps (State, Affix) → {Set of Next States} (One-to-many relation)
*   **q0**: Initial state (Unprocessed Word)
*   **F**: Set of Accepting States (where `RemainingString` ∈ Lexicon)

### Implementation

**Use Python (Prototype) & C (Core Engine) for development:**

*   **Python Wrapper**: Handles API requests, JSON serving, and basic rule-based analysis.
*   **C Engine**: Implements the NFA/BFS logic for high-performance morphological parsing.
*   **Dual-Dictionary Lookup**: Simultaneous verification against Bisaya and Tagalog dictionaries to detect code-switching at the word level.

**Core Algorithm: BFS Morphological Parse**

Input: word (string)
Output: List of {root, affixes[], language}
1.  Initialize Queue `Q` with `StartState(word)`
2.  While `Q` is not empty:
    a.  `Current` ← Dequeue `Q`
    b.  **Root Check**: If `Current.str` in Dictionary → Add to Results
    c.  **Branch Prefix**: Find all `p` in `Prefixes` where `Current.str` starts with `p` → Enqueue `(Current.str - p)`
    d.  **Branch Suffix**: Find all `s` in `Suffixes` where `Current.str` ends with `s` → Enqueue `(Current.str - s)`
    e.  **Branch Infix**: Find all `i` in `Infixes` inside `Current.str` → Enqueue `(Current.str - i)`
3.  Return collected Results

### Testing and Evaluation

**Evaluate the system using:**

*   Sample Bisaya-Tagalog code-switched sentences with known morphological structures
*   Measure morphological decomposition accuracy (correct prefix-root-suffix identification)
*   Measure language identification accuracy (correct Bisaya/Tagalog/Both tagging)
*   Measure code-switching detection precision (correct switch point identification)
*   Measure response time and computational efficiency

**Test Dataset:**

*   50-100 manually annotated sentences
*   Mix of monolingual Bisaya, monolingual Tagalog, and code-switched utterances
*   Known morphological structures (gold standard)

**Evaluation Metrics**  
Decomposition Accuracy = (Correctly Decomposed Words) / (Total Words) × 100%  
Language ID Accuracy = (Correctly Tagged Words) / (Total Words) × 100%  
Switch Detection Precision = True Positives / (True Positives + False Positives)

## VI. CALENDAR OF ACTIVITIES

| Tasks | Week 1 | Week 2 | Week 3 | Week 4 | Week 5 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| Topic approval and literature review | ✓ |  |  |  |  |
| Data collection and lexicon creation | ✓ | ✓ |  |  |  |
| **NFA design and lookup table structure** |  | ✓ | ✓ |  |  |
| Implementation of core automata functions |  |  | ✓ | ✓ |  |
| Language tagging and code-switch detection |  |  |  | ✓ |  |
| Testing and refinement |  |  |  | ✓ | ✓ |
| Evaluation and documentation |  |  |  |  | ✓ |
| Final presentation and submission |  |  |  |  | ✓ |

## VII. REFERENCES

Beesley, K. R., & Karttunen, L. (2003). *Finite State Morphology*. CSLI Publications.

Cheng, Y., Zhang, Y., & Wang, X. (2020). Code-switching detection using finite automata with character-level features. *Proceedings of the International Conference on Computational Linguistics*, 234-245.

Lindén, K., Silfverberg, M., & Pirinen, T. (2009). HFST tools for morphology—An efficient open-source package for construction of morphological analyzers. In *State of the Art in Computational Morphology* (pp. 28-47). Springer.

Mohri, M. (2020). Finite-state transducers and their applications in natural language processing. *Journal of Computational Linguistics*, 46(1), 1–22.

Roxas, R. E. (2007). Morphological analyzer for Tagalog using affix-stripping algorithm. *Philippine Journal of Linguistics*, 38(2), 45-62.

Sipser, M. (2013). *Introduction to the Theory of Computation* (3rd ed.). Cengage Learning.
