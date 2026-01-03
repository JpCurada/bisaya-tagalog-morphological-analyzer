/**
 * Analyzer Page JavaScript
 * Handles text analysis with comprehensive morphological breakdown
 */

// =====================================================
// Initialization
// =====================================================

document.addEventListener('DOMContentLoaded', () => {
    loadQuickStats();

    // Add keyboard shortcut
    document.getElementById('inputText').addEventListener('keypress', (e) => {
        if (e.key === 'Enter' && e.ctrlKey) {
            analyzeText();
        }
    });
});

// =====================================================
// Quick Stats Loading
// =====================================================

async function loadQuickStats() {
    try {
        const response = await fetch('/api/stats');
        const stats = await response.json();

        document.getElementById('qsPrefixes').textContent = stats.prefixes;
        document.getElementById('qsSuffixes').textContent = stats.suffixes;
        document.getElementById('qsRoots').textContent = formatNumber(stats.total_roots);
    } catch (error) {
        console.log('Stats loading skipped');
    }
}

function formatNumber(num) {
    if (num >= 1000) {
        return (num / 1000).toFixed(1) + 'k';
    }
    return num.toString();
}

// =====================================================
// Analysis Functions
// =====================================================

async function analyzeText() {
    const text = document.getElementById('inputText').value;
    if (!text.trim()) {
        showToast('Please enter some text to analyze.', 'error');
        return;
    }

    const loadSpan = document.getElementById('loading');
    const resultSec = document.getElementById('resultsSection');

    loadSpan.classList.remove('hidden');
    resultSec.classList.add('hidden');

    loadSpan.scrollIntoView({ behavior: 'smooth', block: 'center' });

    try {
        const response = await fetch('/api/analyze', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ text })
        });

        const data = await response.json();

        // Small delay for UX
        await new Promise(r => setTimeout(r, 400));

        renderResults(data);
        loadSpan.classList.add('hidden');
        resultSec.classList.remove('hidden');

        resultSec.scrollIntoView({ behavior: 'smooth', block: 'start' });

    } catch (error) {
        console.error('Error:', error);
        showToast('An error occurred during analysis.', 'error');
        loadSpan.classList.add('hidden');
    }
}

function clearText() {
    document.getElementById('inputText').value = '';
    document.getElementById('resultsSection').classList.add('hidden');
    document.getElementById('inputText').focus();
}

// =====================================================
// Rendering
// =====================================================

function renderResults(data) {
    // Update stats
    animateValue(document.getElementById('statTotal'), 0, data.stats.total_words, 800);

    const validPerc = data.stats.total_words > 0
        ? Math.round((data.stats.valid_words / data.stats.total_words) * 100)
        : 0;
    document.getElementById('statValid').textContent = validPerc + '%';

    animateValue(document.getElementById('statSwitches'), 0, data.stats.switch_count, 800);

    // Update result count
    document.getElementById('resultCount').textContent =
        `${data.results.length} word${data.results.length !== 1 ? 's' : ''}`;

    // Render flow visualization
    renderFlowVisualization(data.results);

    // Render cards
    renderCards(data.results);
}

function renderFlowVisualization(results) {
    const flowContainer = document.getElementById('flowVis');
    flowContainer.innerHTML = '';

    results.forEach(res => {
        // Create flow item with structure
        const flowItem = document.createElement('div');
        flowItem.className = `flow-item tag-${res.language}`;

        let components = [];

        // Add prefixes
        if (res.prefixes && res.prefixes.length > 0) {
            res.prefixes.forEach(p => {
                components.push(`<span class="flow-comp prefix">${p}-</span>`);
            });
        }

        // Add root
        if (res.root) {
            components.push(`<span class="flow-comp root">${res.root}</span>`);
        } else if (!res.valid) {
            components.push(`<span class="flow-comp unknown">${res.word}</span>`);
        }

        // Add suffixes
        if (res.suffixes && res.suffixes.length > 0) {
            res.suffixes.forEach(s => {
                components.push(`<span class="flow-comp suffix">-${s}</span>`);
            });
        }

        flowItem.innerHTML = components.join('');

        // Add tooltip with first definition
        let tooltipText = res.language;
        if (res.definitions && res.definitions.length > 0) {
            const firstDef = res.definitions[0].definition || '';
            tooltipText += ': ' + firstDef.substring(0, 50) + (firstDef.length > 50 ? '...' : '');
            if (res.definitions.length > 1) {
                tooltipText += ` (+${res.definitions.length - 1} more)`;
            }
        }
        flowItem.title = tooltipText;

        flowContainer.appendChild(flowItem);
    });
}

function renderCards(results) {
    const cardsContainer = document.getElementById('cardsContainer');
    cardsContainer.innerHTML = '';

    results.forEach((res, index) => {
        const card = createMorphCard(res, index);
        cardsContainer.appendChild(card);
    });
}

function createMorphCard(res, index) {
    const card = document.createElement('div');
    card.className = `morph-card ${res.valid ? 'valid' : 'invalid'} tag-${res.language}`;
    card.style.animationDelay = `${index * 0.08}s`;

    // Build structure display
    let structureHtml = '';

    // Prefixes
    if (res.prefixes && res.prefixes.length > 0) {
        res.prefixes.forEach(p => {
            structureHtml += `<span class="comp-prefix" title="Prefix">${p}-</span>`;
            structureHtml += `<span class="comp-sep">+</span>`;
        });
    }

    // Root
    if (res.root) {
        structureHtml += `<span class="comp-root" title="Root">${res.root}</span>`;
    } else {
        structureHtml += `<span class="comp-unknown">${res.word}</span>`;
    }

    // Suffixes
    if (res.suffixes && res.suffixes.length > 0) {
        res.suffixes.forEach(s => {
            structureHtml += `<span class="comp-sep">+</span>`;
            structureHtml += `<span class="comp-suffix" title="Suffix">-${s}</span>`;
        });
    }

    // Build affix functions section
    let affixFunctionsHtml = '';
    if (res.affix_functions && res.affix_functions.length > 0) {
        affixFunctionsHtml = `
            <div class="detail-row">
                <span class="label">Affix Functions</span>
                <div class="affix-functions">
                    ${res.affix_functions.map(af => `
                        <div class="affix-fn">
                            <span class="affix-fn-key">${af.affix}</span>
                            <span class="affix-fn-val">${truncate(af.function, 60)}</span>
                        </div>
                    `).join('')}
                </div>
            </div>
        `;
    }

    // Build definitions section (now shows ALL dictionary entries)
    let definitionsHtml = '';
    if (res.definitions && res.definitions.length > 0) {
        definitionsHtml = `
            <div class="detail-row">
                <span class="label">Definitions</span>
                <div class="definitions-list">
                    ${res.definitions.map(def => `
                        <div class="definition-entry">
                            <span class="def-lang-badge ${def.language.toLowerCase()}">${def.language}</span>
                            <div class="def-content">
                                <span class="def-text">${truncate(def.definition, 150)}</span>
                                ${def.pos ? `<span class="def-pos">${def.pos}</span>` : ''}
                                ${def.link ? `<a href="${def.link}" target="_blank" class="def-link" title="View source">↗</a>` : ''}
                            </div>
                        </div>
                    `).join('')}
                </div>
            </div>
        `;
    }

    card.innerHTML = `
        <div class="word-header">
            <span class="word-text">${res.word}</span>
            <span class="lang-badge">${res.language}</span>
        </div>
        <div class="morph-details">
            <div class="detail-row">
                <span class="label">Structure</span>
                <div class="structure-display">${structureHtml}</div>
            </div>
            ${definitionsHtml}
            ${affixFunctionsHtml}
        </div>
    `;

    return card;
}

// =====================================================
// Utility Functions
// =====================================================

function truncate(str, maxLen) {
    if (!str) return '';
    if (str.length <= maxLen) return str;
    return str.substring(0, maxLen) + '...';
}

function animateValue(obj, start, end, duration) {
    let startTimestamp = null;
    const step = (timestamp) => {
        if (!startTimestamp) startTimestamp = timestamp;
        const progress = Math.min((timestamp - startTimestamp) / duration, 1);
        obj.innerHTML = Math.floor(progress * (end - start) + start);
        if (progress < 1) {
            window.requestAnimationFrame(step);
        }
    };
    window.requestAnimationFrame(step);
}

function showToast(message, type = 'info') {
    const container = document.getElementById('toast-container');
    const toast = document.createElement('div');
    toast.className = `toast toast-${type}`;
    toast.textContent = message;

    container.appendChild(toast);

    // Animate in
    requestAnimationFrame(() => {
        toast.style.opacity = '1';
        toast.style.transform = 'translateX(0)';
    });

    // Auto remove
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transform = 'translateX(100%)';
        setTimeout(() => toast.remove(), 300);
    }, 4000);
}
