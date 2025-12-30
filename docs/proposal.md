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

This project aims to construct a **lexicon-based morphological analyzer using Deterministic Finite Automata (DFA) with integrated lookup table architecture** that decomposes Bisaya-Tagalog code-switched words into morphological components, identifies the language of each token, and detects code-switching points. By grounding the project in automata theory, it directly demonstrates how formal language concepts can be applied to practical morphological analysis systems for low-resource bilingual contexts.

## **II. RELATED WORKS**

Morphological analysis systems have become an essential component of natural language processing tools across various languages. Morphological analyzers commonly rely on foundational concepts from automata theory, particularly the use of **Deterministic Finite Automata (DFA)** for morphological decomposition and **lookup tables** for lexical validation. These methods have been extensively studied and applied across various languages; however, bilingual morphological analysis for Philippine languages, particularly code-switching contexts, remains largely unexplored.

**Beesley and Karttunen (2003)** pioneered finite-state morphological analysis using Finite-State Transducers (FST). Their XEROX morphological analyzer demonstrated that complex morphological processes can be modeled as cascades of finite-state operations. They showed that affix stripping, root lookup, and morphological rule application can be unified under finite-state framework with lookup table integration. Their work achieved 94% accuracy on English morphological analysis and established the theoretical foundation for FST-based morphology.

Several studies highlight the role of **finite automata in efficient morphological recognition**. For example, **tries and DFA-based lexicons** provide deterministic lookup processes that operate in linear time relative to word length, making them optimal for real-time morphological analysis applications (Mohri, 2020). These automata can represent large dictionaries while sharing common prefixes, significantly reducing storage requirements. Such principles form the basis of modern lexicon-driven morphological analyzers.

**Lindén et al. (2009)** extended finite-state approaches to agglutinative languages with rich morphology, developing HFST (Helsinki Finite-State Technology) toolkit. They demonstrated that highly inflected languages like Finnish and Turkish can be analyzed using weighted finite-state transducers with lexicon lookup at accepting states. Their key contribution was showing how multiple morphological processes (affixation, vowel harmony, consonant gradation) can be composed into single efficient automaton with integrated lexical databases.

**Cheng et al. (2020)** applied finite automata to code-switching detection in Chinese-English bilingual text, using DFA states to represent language contexts and transition tables to capture switching patterns. They achieved 89% accuracy in language identification using character-level automata with language-specific character class lookup tables. Their work demonstrated that code-switching points can be reliably detected using finite-state models with appropriate feature extraction.

**Roxas (2007)** developed morphological analyzer for Tagalog using affix-stripping algorithm with root word dictionary lookup. While not explicitly using automata, their approach followed finite-state principles: systematic left-to-right affix removal with validation against lexical database. They catalogued 158 Tagalog affixes and achieved 82% accuracy on morphological decomposition. However, their system was monolingual and could not handle code-switching.

Despite these advancements, **no publicly available Bisaya-Tagalog bilingual morphological analyzer currently exists**, highlighting a clear gap in digital language support. Existing morphological analyzers for Philippine languages process single languages only and do not include code-switching detection capabilities. This lack of representation in computational tools creates difficulties for researchers and language workers who need to analyze mixed Bisaya-Tagalog text computationally.

## **III. OBJECTIVES OF THE PROJECT**

### A. General Objective

This project will utilize a **Deterministic Finite Automaton (DFA) with integrated lookup table architecture** to decompose Bisaya-Tagalog code-switched words into morphological components (prefix, root, suffix), identify the language of each token (Bisaya, Tagalog, or Shared), and detect code-switching boundaries in mixed-language text.

### B. Specific Objectives

1. **To develop bilingual lexicon lookup tables** containing:  
* Bisaya root words (minimum 200 entries)  
* Tagalog root words (minimum 200 entries)  
* Shared vocabulary (minimum 50 entries)  
* Bisaya affixes in prefix and suffix tables  
* Tagalog affixes in prefix and suffix tables  
    
2. **To construct a DFA for morphological parsing** where:  
* States represent parsing stages (start, prefix-identified, root-search, suffix-identified, accept/reject)  
* Transitions are triggered by lookup table matches (affix tables, root lexicon tables)  
* Accepting states validate complete morphological decomposition  
    
3. **To implement an affix-stripping algorithm** using automata transitions:  
* Prefix stripping transitions guided by prefix lookup table  
* Root identification validated by root lexicon lookup  
* Suffix stripping transitions guided by suffix lookup table  
    
4. **To develop a language identification module** that:  
* Tags each word as Bisaya, Tagalog, or Shared using lexicon lookup  
* Identifies code-switching points (transitions between languages)  
* Generates language sequence annotations  
    
5. **To create a transition table representation** showing:  
* State transitions based on morphological patterns  
* Lookup table queries at each state  
* Decision logic for accepting/rejecting word forms  
    
6. **To evaluate system performance** on:  
* Morphological decomposition accuracy (target: ≥85%)  
* Language identification accuracy (target: ≥90%)  
* Code-switching detection precision (target: ≥80%)

## IV. SCOPE AND LIMITATIONS OF THE PROJECT

### Scope

The project focuses on developing a morphological analyzer that performs word-level Bisaya-Tagalog morphological decomposition using **deterministic finite automata (DFA)** for systematic affix stripping and **lookup tables** for root word validation and language identification. It operates on tokenized text and relies on curated bilingual lexicons stored in hash table data structures.

**Linguistic Coverage:**

* Bisaya (Cebuano) morphology: common affixes (pag-, nag-, mag-, \-an, \-on, \-in, gi-, ma-, maN-)  
* Tagalog morphology: common affixes (mag-, nag-, um-, \-an, \-in, \-han, pag-, paN-)  
* Root word lexicons: 200+ words per language covering high-frequency vocabulary  
* Shared vocabulary: 50+ cognates and borrowings (e.g., "tubig", "libro", "luto", "tulog")

**Morphological Processes Covered:**

* Prefixation: single prefix attachment (pag-kaon, mag-luto)  
* Suffixation: single suffix attachment (kaon-an, luto-han)  
* Circumfixation: prefix \+ suffix combinations (pag-kaon-an, mag-luto-han)  
* Root word identification: validation against bilingual lexicons

**Automata Architecture:**

* DFA structure: 5-7 states representing morphological parsing stages  
* Lookup tables: prefix table (\~30-40 entries), suffix table (\~30-40 entries), root lexicons (450+ total entries)  
* Transition table: complete state transition matrix with lookup conditions  
* Language tagging: per-word language identification (Bisaya/Tagalog/Shared)

### Limitations

However, the system is limited to identifying morphological structures at the word level and **does not handle grammar, syntax, or semantic analysis**. Its accuracy depends heavily on the completeness and quality of the Bisaya-Tagalog lexicon dataset, and it may not recognize:

* **Morphophonemic changes**: allomorphic variations (e.g., paN- → pam-/pan-) simplified or excluded  
* **Reduplication**: forms like "kaon-kaon" (partial reduplication) not processed  
* **Infixation**: Tagalog infixes like \-um- and \-in- excluded from current scope  
* **Multiple affixation**: only single prefix \+ single suffix supported  
* **Intra-word code-switching**: ambiguous cases like "mag-uli" (Tagalog prefix \+ Bisaya root) excluded  
* **Proper nouns, mixed-language expressions, informal slang**, or newly emerging terms commonly used in online communication  
* **Dialectal variations**: focuses on standard Bisaya (Cebuano) and Manila Tagalog only  
* **Context-free constructs**: cannot handle nested morphological structures (would require PDA)

## **V. SYSTEM DESIGN AND METHODOLOGY**

### System Architecture

![][image2]

### 

### 

### 

### DFA-Based Morphological Recognition

**Figure 1\. DFA for Morphological Parsing (Horizontal)**

![][image3]  
*This illustrates how Bisaya-Tagalog words such as "Nag-kaon" and "mag-luto" are parsed through DFA states with lookup table validation at each stage.*

### Lookup Table Structures

**Table 1: Sample Prefix Lookup Table**

| Prefix | Language | Meaning | Example |
| :---- | :---- | :---- | :---- |
| mag- | Tagalog | Actor focus | mag-luto |
| nag- | Both | Actor focus (completed) | nag-luto, nag-kaon |
| pag- | Both | Nominalization | pag-kaon, pag-kain |
| gi- | Bisaya | Object focus | gi-kaon |
| um- | Tagalog | Actor focus | um-alis |
| ma- | Both | Ability/State | ma-kaon, ma-kain |

*Target: 30-40 prefix entries covering both languages*

**Table 2: Sample Suffix Lookup Table**

| Suffix | Language | Meaning | Example |
| :---- | :---- | :---- | :---- |
| \-an | Both | Locative focus | kaon-an, kain-an |
| \-on | Bisaya | Object focus | kaon-on |
| \-in | Tagalog | Object focus | kain-in |
| \-han | Tagalog | Locative/Benefactive | luto-han |
| \-a | Bisaya | Imperative | kaon-a |

*Target: 30-40 suffix entries covering both languages*

**Table 3: Sample Root Lexicons**

| Bisaya Roots | Tagalog Roots | Shared Vocabulary |
| :---- | :---- | :---- |
| kaon (eat) | kain (eat) | luto (cook) |
| balay (house) | bahay (house) | tulog (sleep) |
| dako (big) | malaki (big) | tubig (water) |
| gamay (small) | maliit (small) | libro (book) |
| ayo (good) | mabuti (good) | eskwela (school) |

*Target: 200+ Bisaya, 200+ Tagalog, 50+ Shared entries*

### Data Collection and Processing

**Data Collection:**

* Gather Bisaya words from publicly available digital dictionaries (Cebuano-English dictionaries), linguistic archives, and academic sources  
* Gather Tagalog words from TagDict, Filipino linguistic resources, and lexicographic databases  
* Identify shared vocabulary through cognate analysis and loanword documentation  
* Collect affix inventories from linguistic descriptions of both languages

**Data Processing:**\\

* Clean and format collected words into structured lexicon files (JSON format)  
* Organize into separate tables: prefix\_table.json, suffix\_table.json, bisaya\_roots.json, tagalog\_roots.json, shared\_vocab.json  
* Implement hash table data structures for O(1) lookup performance  
* Validate lexicon entries for consistency and accuracy

### Automata Construction

**Build a DFA where:**

* Each valid morphological structure corresponds to an accepted path  
* States represent parsing stages (q0: start, q1: prefix-stripped, q2: root-found, q3: suffix-stripped, q4: accept)  
* Transitions are triggered by successful lookup table queries  
* Lookup tables serve as "extended alphabet" \- membership in table determines transitions

**Formal DFA Definition:**

DFA\_Morph \= (Q, Σ, δ, q0, F)

Q \= {q0, q1, q2, q3, q4, q\_reject}  
Σ \= Character alphabet \+ {PREFIX\_MATCH, ROOT\_MATCH, SUFFIX\_MATCH}  
δ \= Transition function (see Transition Table)  
q0 \= q0 (initial state)  
F \= {q4} (accepting state)

**Transition Table:**

| Current State | Input Condition | Lookup Query | Next State | Action |
| :---- | :---- | :---- | :---- | :---- |
| q0 | Word begins with known prefix | Query Prefix Table | q1 | Strip prefix, store it |
| q0 | No prefix match | \- | q1 | No prefix (NULL) |
| q1 | Remaining in Root Lexicon | Query B/T/S Tables | q2 | Identify root, tag language |
| q1 | No root match | \- | q\_reject | Invalid word |
| q2 | Remaining in Suffix Table | Query Suffix Table | q3 | Strip suffix, store it |
| q2 | No remaining chars | \- | q3 | No suffix (NULL) |
| q3 | Components valid | Validate compatibility | q4 | Accept, output |
| q3 | Invalid morphology | \- | q\_reject | Reject |

### Implementation

**Use Python for automaton logic development:**

* Implement DFA state machine class structure  
* Implement hash table lookup functions for prefix, suffix, root tables  
* Develop morphological parsing algorithm using state transitions  
* Implement language tagging based on root lexicon membership  
* Develop code-switching detection through language sequence tracking

**Core Algorithm: Morphological Parse**

Input: word (string)  
Output: {prefix, root, suffix, language, validity}  
1\. current\_state ← q0  
2\. Check PREFIX\_TABLE → transition to q1  
3\. Check ROOT\_LEXICONS → transition to q2  
4\. Check SUFFIX\_TABLE → transition to q3  
5\. Validate morphology → transition to q4 or q\_reject  
6\. Return analysis result

### Testing and Evaluation

**Evaluate the system using:**

* Sample Bisaya-Tagalog code-switched sentences with known morphological structures  
* Measure morphological decomposition accuracy (correct prefix-root-suffix identification)  
* Measure language identification accuracy (correct Bisaya/Tagalog/Shared tagging)  
* Measure code-switching detection precision (correct switch point identification)  
* Measure response time and computational efficiency

**Test Dataset:**

* 50-100 manually annotated sentences  
* Mix of monolingual Bisaya, monolingual Tagalog, and code-switched utterances  
* Known morphological structures (gold standard)

**Evaluation Metrics**  
Decomposition Accuracy \= (Correctly Decomposed Words) / (Total Words) × 100%  
Language ID Accuracy \= (Correctly Tagged Words) / (Total Words) × 100%  
Switch Detection Precision \= True Positives / (True Positives \+ False Positives)

## VI. CALENDAR OF ACTIVITIES

| Tasks | Week 1 | Week 2 | Week 3 | Week 4 | Week 5 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| Topic approval and literature review | ✓ |  |  |  |  |
| Data collection and lexicon creation | ✓ | ✓ |  |  |  |
| DFA design and lookup table structure |  | ✓ | ✓ |  |  |
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
