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

    // Smooth scroll to loading
    loadSpan.scrollIntoView({ behavior: 'smooth', block: 'center' });

    try {
        const response = await fetch('/api/analyze', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ text })
        });

        const data = await response.json();

        // Artificial delay for UX (so specific loading animation can be seen)
        // awaits are cheap here
        await new Promise(r => setTimeout(r, 600));

        renderResults(data);
        loadSpan.classList.add('hidden');
        resultSec.classList.remove('hidden');

        // Scroll to results
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

function renderResults(data) {
    // Stats Animation
    animateValue(document.getElementById('statTotal'), 0, data.stats.total_words, 1000);
    animateValue(document.getElementById('statSwitches'), 0, data.stats.switch_count, 1000);

    const validPerc = data.stats.total_words > 0
        ? Math.round((data.stats.valid_words / data.stats.total_words) * 100)
        : 0;
    document.getElementById('statValid').textContent = validPerc + '%';

    // Flow Visualization - Show individual morphemes
    const flowContainer = document.getElementById('flowVis');
    flowContainer.innerHTML = '';
    data.results.forEach(res => {
        // Create separate bubbles for prefix, root, and suffix
        if (res.prefix) {
            const prefixSpan = document.createElement('span');
            prefixSpan.className = `flow-item tag-${res.language}`;
            prefixSpan.innerHTML = `<span class="flow-comp prefix">${res.prefix}</span>`;
            prefixSpan.title = `Prefix from prefix_table.json`;
            flowContainer.appendChild(prefixSpan);
        }

        if (res.root) {
            const rootSpan = document.createElement('span');
            rootSpan.className = `flow-item tag-${res.language}`;
            rootSpan.innerHTML = `<span class="flow-comp root">${res.root}</span>`;
            // Determine source based on language
            let source = 'shared_vocab.json';
            if (res.language === 'Bisaya') source = 'bisaya_roots.json';
            else if (res.language === 'Tagalog') source = 'tagalog_roots.json';
            rootSpan.title = `Root from ${source}`;
            flowContainer.appendChild(rootSpan);
        } else {
            // Unknown word - show as-is
            const unknownSpan = document.createElement('span');
            unknownSpan.className = `flow-item tag-Unknown`;
            unknownSpan.innerHTML = `<span class="flow-comp root">${res.original_token || res.word}</span>`;
            unknownSpan.title = 'Unrecognized word';
            flowContainer.appendChild(unknownSpan);
        }

        if (res.suffix) {
            const suffixSpan = document.createElement('span');
            suffixSpan.className = `flow-item tag-${res.language}`;
            suffixSpan.innerHTML = `<span class="flow-comp suffix">${res.suffix}</span>`;
            suffixSpan.title = `Suffix from suffix_table.json`;
            flowContainer.appendChild(suffixSpan);
        }
    });

    // Cards
    const cardsContainer = document.getElementById('cardsContainer');
    cardsContainer.innerHTML = '';

    data.results.forEach((res, index) => {
        const card = document.createElement('div');
        card.className = `morph-card ${res.valid ? 'valid' : 'invalid'}`;
        card.style.animationDelay = `${index * 0.1}s`;

        const langClass = `tag-${res.language}`;

        let components = [];
        // Styling tokens
        const pStyle = 'color: #fdcb6e; font-weight: 600';
        const rStyle = 'color: #fff; text-decoration: underline; font-weight: 700';
        const sStyle = 'color: #fdcb6e; font-weight: 600';

        if (res.prefix) components.push(`<span title="Prefix" style="${pStyle}">${res.prefix}</span>`);
        if (res.root) components.push(`<span title="Root" style="${rStyle}">${res.root}</span>`);
        if (res.suffix) components.push(`<span title="Suffix" style="${sStyle}">${res.suffix}</span>`);

        const componentsHtml = components.length > 0
            ? components.join(' + ')
            : '<span style="opacity:0.5; font-style:italic">Unrecognized structure</span>';

        // Build meaning/POS display
        let meaningHtml = '';
        if (res.meaning) {
            meaningHtml = `
                <div class="detail-row">
                    <span class="label">Meaning</span>
                    <span style="color: #a29bfe; font-weight: 500;">${res.meaning}</span>
                </div>
            `;
        }

        let posHtml = '';
        if (res.pos) {
            posHtml = `
                <div class="detail-row">
                    <span class="label">Part of Speech</span>
                    <span style="color: #b0b3c5; text-transform: capitalize;">${res.pos}</span>
                </div>
            `;
        }

        let originHtml = '';
        if (res.origin) {
            originHtml = `
                <div class="detail-row">
                    <span class="label">Origin</span>
                    <span style="color: #fdcb6e; font-weight: 500;">${res.origin}</span>
                </div>
            `;
        }

        card.innerHTML = `
            <div class="word-header">
                ${res.word}
                <span class="lang-badge ${langClass}">${res.language}</span>
            </div>
            <div class="morph-details">
                <div class="detail-row">
                    <span class="label">Composition</span>
                    <span>${componentsHtml}</span>
                </div>
                ${meaningHtml}
                ${posHtml}
                ${originHtml}
            </div>
        `;
        cardsContainer.appendChild(card);
    });
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
    toast.className = `toast ${type}`;
    toast.textContent = message;

    container.appendChild(toast);

    // Auto remove
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transform = 'translateY(20px)';
        setTimeout(() => toast.remove(), 300);
    }, 4000);
}

// Add Enter key support
document.getElementById('inputText').addEventListener('keypress', function (e) {
    if (e.key === 'Enter' && e.ctrlKey) {
        analyzeText();
    }
});
