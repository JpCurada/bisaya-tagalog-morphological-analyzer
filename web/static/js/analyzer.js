/**
 * Bistag Analyzer - Main Application Logic
 * Handles morphological analysis and visualization
 */

// State Management
const AnalyzerState = {
    currentResults: null,
    isAnalyzing: false
};

// UI Elements (cached for performance)
const UI = {
    inputText: null,
    loading: null,
    resultsSection: null,
    statTotal: null,
    statSwitches: null,
    statValid: null,
    flowVis: null,
    cardsContainer: null,

    init() {
        this.inputText = DOM.qs('#inputText');
        this.loading = DOM.qs('#loading');
        this.resultsSection = DOM.qs('#resultsSection');
        this.statTotal = DOM.qs('#statTotal');
        this.statSwitches = DOM.qs('#statSwitches');
        this.statValid = DOM.qs('#statValid');
        this.flowVis = DOM.qs('#flowVis');
        this.cardsContainer = DOM.qs('#cardsContainer');
    }
};

// Main Analysis Function
async function analyzeText() {
    const text = UI.inputText.value.trim();

    if (!text) {
        Toast.error('Please enter some text to analyze.');
        return;
    }

    if (AnalyzerState.isAnalyzing) return;

    AnalyzerState.isAnalyzing = true;
    UI.loading.classList.remove('hidden');
    UI.resultsSection.classList.add('hidden');
    DOM.scrollTo(UI.loading, { block: 'center' });

    try {
        const data = await API.analyze(text);

        // Small delay for UX
        await new Promise(resolve => setTimeout(resolve, 600));

        AnalyzerState.currentResults = data;
        renderResults(data);

        UI.loading.classList.add('hidden');
        UI.resultsSection.classList.remove('hidden');
        DOM.scrollTo(UI.resultsSection);

    } catch (error) {
        Toast.error('An error occurred during analysis.');
        console.error('Analysis error:', error);
    } finally {
        AnalyzerState.isAnalyzing = false;
        UI.loading.classList.add('hidden');
    }
}

// Clear Input
function clearText() {
    UI.inputText.value = '';
    UI.resultsSection.classList.add('hidden');
    UI.inputText.focus();
}

// Render Results
function renderResults(data) {
    renderStats(data.stats);
    renderLanguageFlow(data.results);
    renderMorphologicalCards(data.results);
}

// Render Statistics
function renderStats(stats) {
    DOM.animateValue(UI.statTotal, 0, stats.total_words, 1000);
    DOM.animateValue(UI.statSwitches, 0, stats.switch_count, 1000);

    const validPerc = stats.total_words > 0
        ? Math.round((stats.valid_words / stats.total_words) * 100)
        : 0;
    UI.statValid.textContent = validPerc + '%';
}

// Render Language Flow
function renderLanguageFlow(results) {
    UI.flowVis.innerHTML = '';

    results.forEach(res => {
        // Create separate bubbles for prefix, root, and suffix
        if (res.prefix) {
            const prefixSpan = createFlowItem(res.prefix, 'prefix', res.language, 'Prefix from prefix_table.json');
            UI.flowVis.appendChild(prefixSpan);
        }

        if (res.root) {
            const source = getSourceFile(res.language);
            const rootSpan = createFlowItem(res.root, 'root', res.language, `Root from ${source}`);
            UI.flowVis.appendChild(rootSpan);
        } else {
            const unknownSpan = createFlowItem(
                res.original_token || res.word,
                'root',
                'Unknown',
                'Unrecognized word'
            );
            UI.flowVis.appendChild(unknownSpan);
        }

        if (res.suffix) {
            const suffixSpan = createFlowItem(res.suffix, 'suffix', res.language, 'Suffix from suffix_table.json');
            UI.flowVis.appendChild(suffixSpan);
        }
    });
}

// Create Flow Item
function createFlowItem(text, type, language, title) {
    const span = DOM.create('span', {
        className: `flow-item tag-${language}`,
        title: title
    });

    const comp = DOM.create('span', {
        className: `flow-comp ${type}`
    }, [text]);

    span.appendChild(comp);
    return span;
}

// Get Source File Name
function getSourceFile(language) {
    const sourceMap = {
        'Bisaya': 'bisaya_roots.json',
        'Tagalog': 'tagalog_roots.json',
        'Shared': 'shared_vocab.json'
    };
    return sourceMap[language] || 'shared_vocab.json';
}

// Render Morphological Cards
function renderMorphologicalCards(results) {
    UI.cardsContainer.innerHTML = '';

    results.forEach((res, index) => {
        const card = createMorphCard(res, index);
        UI.cardsContainer.appendChild(card);
    });
}

// Create Morphological Card
function createMorphCard(res, index) {
    const card = DOM.create('div', {
        className: `morph-card ${res.valid ? 'valid' : 'invalid'}`,
        style: `animation-delay: ${index * 0.1}s`
    });

    const langClass = `tag-${res.language}`;

    // Build composition HTML
    const components = [];
    if (res.prefix) components.push(`<span class="comp-prefix" title="Prefix">${res.prefix}</span>`);
    if (res.root) components.push(`<span class="comp-root" title="Root">${res.root}</span>`);
    if (res.suffix) components.push(`<span class="comp-suffix" title="Suffix">${res.suffix}</span>`);

    const componentsHtml = components.length > 0
        ? components.join(' <span class="comp-sep">+</span> ')
        : '<span class="comp-unknown">Unrecognized structure</span>';

    // Build info rows
    const infoRows = [];

    infoRows.push(`
        <div class="detail-row">
            <span class="label">Composition</span>
            <span class="value">${componentsHtml}</span>
        </div>
    `);

    if (res.meaning) {
        infoRows.push(`
            <div class="detail-row">
                <span class="label">Meaning</span>
                <span class="value meaning">${res.meaning}</span>
            </div>
        `);
    }

    if (res.pos) {
        infoRows.push(`
            <div class="detail-row">
                <span class="label">Part of Speech</span>
                <span class="value pos">${res.pos}</span>
            </div>
        `);
    }

    if (res.origin) {
        infoRows.push(`
            <div class="detail-row">
                <span class="label">Origin</span>
                <span class="value origin">${res.origin}</span>
            </div>
        `);
    }

    card.innerHTML = `
        <div class="word-header">
            <span class="word-text">${res.word}</span>
            <span class="lang-badge ${langClass}">${res.language}</span>
        </div>
        <div class="morph-details">
            ${infoRows.join('')}
        </div>
    `;

    return card;
}

// Initialize Application
function initAnalyzer() {
    UI.init();

    // Add keyboard shortcut (Ctrl+Enter to analyze)
    UI.inputText.addEventListener('keypress', (e) => {
        if (e.key === 'Enter' && e.ctrlKey) {
            analyzeText();
        }
    });

    // Add input validation
    UI.inputText.addEventListener('input', debounce(() => {
        const text = UI.inputText.value.trim();
        if (text.length > 1000) {
            Toast.show('Text is quite long. Analysis may take a moment.', 'info');
        }
    }, 500));
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initAnalyzer);
} else {
    initAnalyzer();
}
