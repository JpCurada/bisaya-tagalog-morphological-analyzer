/**
 * Browse Page JavaScript
 * Handles affix and root data browsing with API interactions
 */

// State
let currentAffixType = 'prefixes';
let currentRootLanguage = 'all';
let allAffixes = [];
let searchTimeout = null;

// =====================================================
// Initialization
// =====================================================

document.addEventListener('DOMContentLoaded', () => {
    loadStats();
    loadAffixes('prefixes');
});

// =====================================================
// Stats Loading
// =====================================================

async function loadStats() {
    try {
        const response = await fetch('/api/stats');
        const stats = await response.json();

        document.getElementById('totalAffixes').textContent = stats.total_affixes.toLocaleString();
        document.getElementById('totalRoots').textContent = stats.total_roots.toLocaleString();
    } catch (error) {
        console.error('Failed to load stats:', error);
    }
}

// =====================================================
// Tab Switching
// =====================================================

function switchTab(tabId) {
    // Update tab buttons
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.classList.toggle('active', btn.dataset.tab === tabId);
    });

    // Update tab content
    document.querySelectorAll('.tab-content').forEach(content => {
        content.classList.toggle('active', content.id === tabId + 'Tab');
    });

    // Load data if needed
    if (tabId === 'roots') {
        const rootGrid = document.getElementById('rootGrid');
        if (rootGrid.children.length === 0) {
            loadRandomRoots();
        }
    }
}

// =====================================================
// Affix Loading
// =====================================================

async function loadAffixes(type) {
    currentAffixType = type;

    // Update filter buttons
    document.querySelectorAll('.filter-btn[data-affix]').forEach(btn => {
        btn.classList.toggle('active', btn.dataset.affix === type);
    });

    // Show loading
    const grid = document.getElementById('affixGrid');
    grid.innerHTML = '<div class="loading-placeholder">Loading affixes...</div>';

    try {
        const response = await fetch(`/api/affixes/${type}`);
        allAffixes = await response.json();

        document.getElementById('affixCount').textContent =
            `${allAffixes.length} ${type} found`;

        renderAffixes(allAffixes);
    } catch (error) {
        console.error('Failed to load affixes:', error);
        grid.innerHTML = '<div class="error-message">Failed to load affixes</div>';
    }
}

function renderAffixes(affixes) {
    const grid = document.getElementById('affixGrid');
    grid.innerHTML = '';

    affixes.forEach((affix, index) => {
        const card = createAffixCard(affix, index);
        grid.appendChild(card);
    });
}

function createAffixCard(affix, index) {
    const card = document.createElement('div');
    card.className = 'affix-card';
    card.style.animationDelay = `${Math.min(index * 0.05, 0.5)}s`;
    card.onclick = () => showAffixDetail(affix);

    const firstDef = affix.definitions[0] || {};
    const func = firstDef.function || 'No function defined';
    const lang = firstDef.language || 'Unknown';
    const affixType = affix.affix_type || 'affix';

    // Get language class
    let langClass = 'lang-both';
    if (lang === 'Cebuano') langClass = 'lang-cebuano';
    else if (lang === 'Tagalog') langClass = 'lang-tagalog';

    card.innerHTML = `
        <div class="affix-card-header">
            <span class="affix-key">${affix.key}</span>
            <span class="affix-type-badge type-${affixType}">${affixType}</span>
        </div>
        <div class="affix-function">${truncate(func, 100)}</div>
        <div class="affix-meta">
            <span class="meta-tag ${langClass}">${lang}</span>
            ${affix.count > 1 ? `<span class="meta-tag">${affix.count} definitions</span>` : ''}
        </div>
    `;

    return card;
}

function filterAffixes() {
    const query = document.getElementById('affixSearch').value.toLowerCase().trim();

    if (!query) {
        renderAffixes(allAffixes);
        return;
    }

    const filtered = allAffixes.filter(affix => {
        const keyMatch = affix.key.toLowerCase().includes(query);
        const funcMatch = affix.definitions.some(def =>
            (def.function || '').toLowerCase().includes(query)
        );
        return keyMatch || funcMatch;
    });

    renderAffixes(filtered);
    document.getElementById('affixCount').textContent =
        `${filtered.length} of ${allAffixes.length} ${currentAffixType}`;
}

// =====================================================
// Root Loading
// =====================================================

function setRootLanguage(lang) {
    currentRootLanguage = lang;

    // Update filter buttons
    document.querySelectorAll('.filter-btn[data-lang]').forEach(btn => {
        btn.classList.toggle('active', btn.dataset.lang === lang);
    });

    // Re-search if there's a query
    const query = document.getElementById('rootSearch').value.trim();
    if (query) {
        searchRoots();
    } else {
        loadRandomRoots();
    }
}

async function loadRandomRoots() {
    const grid = document.getElementById('rootGrid');
    grid.innerHTML = '<div class="loading-placeholder">Loading sample roots...</div>';

    try {
        const lang = currentRootLanguage === 'all' ? '' : `&language=${currentRootLanguage}`;
        const response = await fetch(`/api/roots/random?count=20${lang}`);
        const roots = await response.json();

        document.getElementById('rootInfo').textContent =
            'Showing random sample. Search to find specific roots.';

        renderRoots(roots);
    } catch (error) {
        console.error('Failed to load random roots:', error);
        grid.innerHTML = '<div class="error-message">Failed to load roots</div>';
    }
}

function searchRoots() {
    // Debounce search
    clearTimeout(searchTimeout);
    searchTimeout = setTimeout(doRootSearch, 300);
}

async function doRootSearch() {
    const query = document.getElementById('rootSearch').value.trim();

    if (!query) {
        loadRandomRoots();
        return;
    }

    const grid = document.getElementById('rootGrid');
    grid.innerHTML = '<div class="loading-placeholder">Searching...</div>';

    try {
        const lang = currentRootLanguage === 'all' ? '' : `&language=${currentRootLanguage}`;
        const response = await fetch(`/api/roots/search?q=${encodeURIComponent(query)}${lang}&limit=50`);
        const roots = await response.json();

        document.getElementById('rootInfo').textContent =
            `Found ${roots.length} roots matching "${query}"`;

        renderRoots(roots);
    } catch (error) {
        console.error('Search failed:', error);
        grid.innerHTML = '<div class="error-message">Search failed</div>';
    }
}

function renderRoots(roots) {
    const grid = document.getElementById('rootGrid');
    grid.innerHTML = '';

    if (roots.length === 0) {
        grid.innerHTML = '<div class="empty-message">No roots found</div>';
        return;
    }

    roots.forEach((root, index) => {
        const card = createRootCard(root, index);
        grid.appendChild(card);
    });
}

function createRootCard(root, index) {
    const card = document.createElement('div');
    card.className = 'root-card';
    card.style.animationDelay = `${Math.min(index * 0.03, 0.5)}s`;
    card.onclick = () => showRootDetail(root);

    const word = root.word || '';
    const definition = root.definition || '';
    const source = root.source || root.language || 'Unknown';
    const pos = root.pos || '';

    // Get language class
    let langClass = 'lang-both';
    if (source.includes('Bisaya') || source.includes('Hiligaynon') || source.includes('Cebuano')) {
        langClass = 'lang-cebuano';
    } else if (source.includes('Tagalog')) {
        langClass = 'lang-tagalog';
    }

    card.innerHTML = `
        <div class="root-word">${word}</div>
        <div class="root-definition">${truncate(definition, 150)}</div>
        <div class="root-meta">
            <span class="meta-tag ${langClass}">${source}</span>
            ${pos ? `<span class="meta-tag">${pos}</span>` : ''}
        </div>
    `;

    return card;
}

// =====================================================
// Detail Modals
// =====================================================

function showAffixDetail(affix) {
    const modal = document.getElementById('detailModal');
    const modalBody = document.getElementById('modalBody');
    const affixType = affix.affix_type || 'affix';

    let definitionsHtml = '';
    affix.definitions.forEach((def, idx) => {
        const derivedTerms = def.derived_terms || [];
        const hasDerivTerms = derivedTerms.length > 0;

        definitionsHtml += `
            <div class="definition-item">
                <div class="def-row">
                    <span class="def-label">Language</span>
                    <span class="def-value">${def.language || 'Unknown'}</span>
                </div>
                ${def.etymology ? `
                <div class="def-row">
                    <span class="def-label">Etymology</span>
                    <span class="def-value">${def.etymology}</span>
                </div>
                ` : ''}
                ${def.pronunciation ? `
                <div class="def-row">
                    <span class="def-label">Pronunciation</span>
                    <span class="def-value pronunciation">${def.pronunciation}</span>
                </div>
                ` : ''}
                ${def.syllabification ? `
                <div class="def-row">
                    <span class="def-label">Syllabification</span>
                    <span class="def-value">${def.syllabification}</span>
                </div>
                ` : ''}
                <div class="def-row">
                    <span class="def-label">Function</span>
                    <span class="def-value function-text">${def.function || 'Not specified'}</span>
                </div>
                ${hasDerivTerms ? `
                <div class="def-row derived-terms-row">
                    <span class="def-label">Derived Terms</span>
                    <div class="derived-terms-list">
                        ${derivedTerms.slice(0, 20).map(term => `<span class="derived-term">${term}</span>`).join('')}
                        ${derivedTerms.length > 20 ? `<span class="derived-term more">+${derivedTerms.length - 20} more</span>` : ''}
                    </div>
                </div>
                ` : ''}
            </div>
        `;
    });

    modalBody.innerHTML = `
        <div class="modal-header">
            <h2 class="modal-title">${affix.key}</h2>
            <div class="modal-subtitle-row">
                <span class="affix-type-badge type-${affixType}">${affixType}</span>
                <span>${affix.count} definition${affix.count > 1 ? 's' : ''}</span>
            </div>
        </div>
        <div class="definition-list">
            ${definitionsHtml}
        </div>
    `;

    modal.classList.remove('hidden');
    document.body.style.overflow = 'hidden';
}

function showRootDetail(root) {
    const modal = document.getElementById('detailModal');
    const modalBody = document.getElementById('modalBody');

    modalBody.innerHTML = `
        <div class="modal-header">
            <h2 class="modal-title">${root.word}</h2>
            <p class="modal-subtitle">${root.source || root.language || 'Unknown'}</p>
        </div>
        <div class="definition-list">
            <div class="definition-item">
                <div class="def-row">
                    <span class="def-label">Definition</span>
                    <span class="def-value">${root.definition || 'No definition available'}</span>
                </div>
                ${root.pos ? `
                <div class="def-row">
                    <span class="def-label">Part of Speech</span>
                    <span class="def-value" style="text-transform: capitalize;">${root.pos}</span>
                </div>
                ` : ''}
                ${root.language ? `
                <div class="def-row">
                    <span class="def-label">Language</span>
                    <span class="def-value">${root.language}</span>
                </div>
                ` : ''}
                ${root.link ? `
                <div class="def-row">
                    <span class="def-label">Reference</span>
                    <a href="${root.link}" target="_blank" class="def-value" style="color: var(--color-accent);">
                        View source →
                    </a>
                </div>
                ` : ''}
            </div>
        </div>
    `;

    modal.classList.remove('hidden');
    document.body.style.overflow = 'hidden';
}

function closeModal() {
    document.getElementById('detailModal').classList.add('hidden');
    document.body.style.overflow = '';
}

// Close modal on Escape key
document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
        closeModal();
    }
});

// =====================================================
// Utility Functions
// =====================================================

function truncate(str, maxLen) {
    if (!str) return '';
    if (str.length <= maxLen) return str;
    return str.substring(0, maxLen) + '...';
}

function showToast(message, type = 'info') {
    const container = document.getElementById('toast-container');
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.textContent = message;

    container.appendChild(toast);

    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transform = 'translateY(20px)';
        setTimeout(() => toast.remove(), 300);
    }, 4000);
}
