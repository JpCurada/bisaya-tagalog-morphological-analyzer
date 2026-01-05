# Documentation

This directory contains project documentation and reference materials.

## Files

### proposal.md
The original project proposal outlining the Bisaya-Tagalog morphological analyzer concept, methodology, and implementation plan.

### copyfish-github.md
Reference documentation from the Copyfish OCR Chrome extension, used as inspiration for the Bistag extension's OCR features and implementation patterns.

## Related Documentation

- **Main README**: See [../README.md](../README.md) for installation and usage
- **Extension README**: See [../extension/README.md](../extension/README.md) for Chrome extension documentation

## System Architecture

The analyzer helps linguistic research by decomposing words using a **Finite-State Morphotactic approach**. The system consists of four interacting components controlled by a global automaton:

1.  **PrefixFSM**: Lexical automaton for prefix tokens.
2.  **InfixFSM**: Handling ε-transitions for inserting morphemes.
3.  **RootLexicon**: Stem lexicon lookup structure for validation.
4.  **SuffixCircumfixFSM**: Lexical automaton for suffixes and circumfix constraints.

This formal architecture transforms the complex problem of code-switching and agglutination into a rigorous state-transition path problem.

## Finite-State Morphotactic Output Format

We represent outputs in a finite-state morphotactic parse format, where morphemes are separated with `+` and annotated with prefix/infix/suffix/root labels derived from the states of the morphotactic FSM.

**Examples:**
*   `magsulat` -> `mag[PFX] + sulat[ROOT]`
*   `sinulat` -> `in[INFX] + sulat[ROOT]`
*   `kasulatan` -> `ka[CIRCUMFIX_PREFIX] + sulat[ROOT] + an[CIRCUMFIX_SUFFIX]`
