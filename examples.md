# Example Inputs for Bisaya-Tagalog Analyzer

These examples utilize the current lexicon database.

## 1. Bisaya Examples
| Input | Parse Result | Notes |
|-------|--------------|-------|
| `nag-kaon` | Prefix: `nag-`, Root: `kaon` | Progressive Actor Focus |
| `gi-hatag` | Prefix: `gi-`, Root: `hatag` | Object Focus |
| `kaon-on` | Root: `kaon`, Suffix: `-on` | Object Focus (Future) |
| `dako nga balay` | `dako` (Adj), `balay` (Noun) | *'nga' will be Unknown* |
| `gamay nga libro` | `gamay` (Adj), `libro` (Noun) | Small book |

## 2. Tagalog Examples
| Input | Parse Result | Notes |
|-------|--------------|-------|
| `mag-lakad` | Prefix: `mag-`, Root: `lakad` | Actor Focus |
| `bahay` | Root: `bahay` | Noun |
| `ma-laki` | Prefix: `ma-`, Root: `laki` | Adjective (Big) |
| `ma-liit` | Prefix: `ma-`, Root: `liit` | Adjective (Small) |

## 3. Shared / Mixed (Code-Switching)
These sentences demonstrate the analyzer's detection capabilities.

**"Nag-luto siya sa bahay"**
- `Nag-luto`: Shared (Prefix `Nag-` + Shared Root `luto`)
- `siya`: Shared (Pronoun)
- `sa`: *Unknown* (Marker not in lexicon)
- `bahay`: Tagalog (Noun)

**"Gi-hatag niya ang libro"**
- `Gi-hatag`: Bisaya (Prefix `Gi-` + Bisaya Root `hatag`)
- `niya`: *Unknown* (Pronoun not in lexicon)
- `ang`: *Unknown* (Marker not in lexicon)
- `libro`: Shared (Spanish loan)

**"Mag-kaon kami"**
- `Mag-kaon`: Bisaya (Prefix `Mag-` + Bisaya Root `kaon`)
- `kami`: Shared (Pronoun "We")

**"Ma-liit ang balay"**
- `Ma-liit`: Tagalog (Prefix `Ma-` + Tagalog Root `liit`)
- `ang`: *Unknown*
- `balay`: Bisaya (Noun)
