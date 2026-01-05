// State Management
let currentState = 'initial';
let extractedText = '';
let analysisResults = null;

// DOM Elements
const states = {
    initial: document.getElementById('initialState'),
    processing: document.getElementById('processingState'),
    ocr: document.getElementById('ocrState'),
    results: document.getElementById('resultsState')
};

const elements = {
    captureBtn: document.getElementById('captureBtn'),
    analyzeBtn: document.getElementById('analyzeBtn'),
    editBtn: document.getElementById('editBtn'),
    backBtn: document.getElementById('backBtn'),
    newCaptureBtn: document.getElementById('newCaptureBtn'),
    viewDetailsBtn: document.getElementById('viewDetailsBtn'),
    autoAnalyze: document.getElementById('autoAnalyze'),
    extractedTextArea: document.getElementById('extractedText'),
    statusText: document.getElementById('statusText'),
    pasteTextBtn: document.getElementById('pasteTextBtn'),
    pasteInEditBtn: document.getElementById('pasteInEditBtn')
};

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    // Event Listeners
    elements.captureBtn.addEventListener('click', startCapture);
    elements.analyzeBtn.addEventListener('click', analyzeText);
    elements.editBtn.addEventListener('click', () => setState('ocr'));
    elements.backBtn.addEventListener('click', () => setState('initial'));
    elements.newCaptureBtn.addEventListener('click', () => setState('initial'));
    elements.newCaptureBtn.addEventListener('click', () => setState('initial'));
    elements.viewDetailsBtn.addEventListener('click', openFullResults);

    // Paste Handlers
    elements.pasteTextBtn.addEventListener('click', handlePaste);
    elements.pasteInEditBtn.addEventListener('click', handlePaste);

    // Load settings
    loadSettings();

    // Restore previous session state
    restoreState();

    // Check for pending screenshot
    checkPendingScreenshot();
});

// Session State Persistence
async function saveState() {
    const state = {
        currentState,
        extractedText,
        analysisResults,
        timestamp: Date.now()
    };

    await chrome.storage.local.set({ sessionState: state });
    console.log('State saved:', currentState);
}

async function restoreState() {
    const result = await chrome.storage.local.get('sessionState');

    if (result.sessionState) {
        const { currentState: savedState, extractedText: savedText, analysisResults: savedResults, timestamp } = result.sessionState;

        // Only restore if session is less than 1 hour old
        const age = Date.now() - timestamp;
        if (age < 3600000) { // 1 hour
            console.log('Restoring state:', savedState);

            extractedText = savedText || '';
            analysisResults = savedResults || null;

            // Restore UI based on saved state
            if (savedState === 'ocr' && extractedText) {
                elements.extractedTextArea.value = extractedText;
                setState('ocr');
            } else if (savedState === 'results' && analysisResults) {
                // Ensure text is also restored if available
                if (extractedText) {
                    elements.extractedTextArea.value = extractedText;
                }
                displayResults(analysisResults);
            }
        } else {
            // Clear old session
            chrome.storage.local.remove('sessionState');
        }
    }
}

// Check for pending screenshot from content script
async function checkPendingScreenshot() {
    const result = await chrome.storage.local.get(['pendingScreenshot', 'screenshotTimestamp']);

    if (result.pendingScreenshot) {
        // Check if screenshot is recent (within last 60 seconds)
        const age = Date.now() - (result.screenshotTimestamp || 0);
        if (age < 60000) {
            console.log('Found pending screenshot, processing...');
            handleScreenshotCapture(result.pendingScreenshot);
            // Clear the pending screenshot
            chrome.storage.local.remove(['pendingScreenshot', 'screenshotTimestamp']);
        }
    }
}

// State Management
function setState(state) {
    // Hide all states
    Object.values(states).forEach(s => s.classList.add('hidden'));

    // Show target state
    if (states[state]) {
        states[state].classList.remove('hidden');
        currentState = state;

        // Save state for session persistence
        saveState();
    }
}

// Settings
function loadSettings() {
    chrome.storage.sync.get(['autoAnalyze'], (result) => {
        if (result.autoAnalyze !== undefined) {
            elements.autoAnalyze.checked = result.autoAnalyze;
        }
    });
}

elements.autoAnalyze.addEventListener('change', (e) => {
    chrome.storage.sync.set({ autoAnalyze: e.target.checked });
});

// Capture Screenshot
async function startCapture() {
    try {
        setState('processing');
        updateStatus('Preparing screenshot tool...');

        // Get current tab
        const [tab] = await chrome.tabs.query({ active: true, currentWindow: true });

        // Check if URL is restricted
        const restrictedProtocols = ['chrome://', 'chrome-extension://', 'edge://', 'about:', 'view-source:'];
        const isRestricted = restrictedProtocols.some(protocol => tab.url.startsWith(protocol));

        if (isRestricted) {
            showError('Cannot capture screenshots on this page.\n\nPlease navigate to a regular webpage (http:// or https://) and try again.\n\nRestricted pages include:\n• chrome:// pages\n• Extension pages\n• Browser settings');
            setState('initial');
            return;
        }

        // Check if it's a Chrome Web Store page
        if (tab.url.includes('chrome.google.com/webstore')) {
            showError('Cannot capture screenshots on Chrome Web Store.\n\nPlease try on a different webpage.');
            setState('initial');
            return;
        }

        // Inject content script for selection
        await chrome.scripting.executeScript({
            target: { tabId: tab.id },
            files: ['content.js']
        });

        // Send message to start selection
        chrome.tabs.sendMessage(tab.id, { action: 'startSelection' }, (response) => {
            if (chrome.runtime.lastError) {
                showError('Failed to start screenshot tool. Please refresh the page and try again.');
                setState('initial');
            }
        });

    } catch (error) {
        console.error('Capture error:', error);
        showError('Failed to capture screenshot: ' + error.message);
        setState('initial');
    }
}

// Listen for screenshot from content script
chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    if (request.action === 'screenshotCaptured') {
        handleScreenshotCapture(request.dataUrl);
    } else if (request.action === 'selectionCancelled') {
        setState('initial');
    }
});


// Handle screenshot capture and OCR
async function handleScreenshotCapture(imageDataUrl) {
    try {
        setState('processing');

        // Log image size for debugging
        const sizeKB = (imageDataUrl.length * 0.75 / 1024).toFixed(2);
        console.log(`Screenshot captured: ${sizeKB}KB`);

        // Perform OCR with status callback
        const text = await performOCR(imageDataUrl, updateStatus);

        if (!text || text.trim().length === 0) {
            updateStatus('OCR result was empty.');
            // Only auto-analyze if we actually got text
            setState('ocr');
            return;
        }

        extractedText = text;
        elements.extractedTextArea.value = extractedText;

        console.log(`OCR completed successfully. Text length: ${text.length} characters`);

        // Auto-analyze if enabled
        if (elements.autoAnalyze.checked) {
            analyzeText();
        } else {
            setState('ocr');
        }

    } catch (error) {
        console.error('OCR error:', error);
        showError(error.message || 'OCR failed after multiple attempts');
        setState('initial');
    }
}

// Analyze Text with retry logic
async function analyzeText() {
    const maxRetries = 2;
    let attempt = 0;

    const text = elements.extractedTextArea.value.trim();

    if (!text) {
        showError('Please enter some text to analyze.');
        return;
    }

    setState('processing');

    while (attempt < maxRetries) {
        try {
            attempt++;
            updateStatus(`Analyzing morphology... (Attempt ${attempt}/${maxRetries})`);

            // Get API URL from storage or use default
            const { apiUrl } = await chrome.storage.sync.get({ apiUrl: 'http://localhost:8000' });

            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), 15000); // 15s timeout

            const response = await fetch(`${apiUrl}/api/analyze`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ text }),
                signal: controller.signal
            });

            clearTimeout(timeoutId);

            if (!response.ok) {
                throw new Error(`API error: ${response.status}`);
            }

            analysisResults = await response.json();
            updateStatus('Analysis complete!');
            displayResults(analysisResults);

            break; // Success, exit loop

        } catch (error) {
            console.error(`Analysis error (attempt ${attempt}):`, error);

            if (error.name === 'AbortError') {
                if (attempt < maxRetries) {
                    updateStatus('Request timeout, retrying...');
                    await new Promise(resolve => setTimeout(resolve, 1000));
                    continue;
                }
                showError('Analysis timed out. The server might be busy or not running.');
            } else if (attempt < maxRetries) {
                updateStatus('Analysis failed, retrying...');
                await new Promise(resolve => setTimeout(resolve, 1000));
            } else {
                showError('Analysis failed. Make sure the Bistag server is running at http://localhost:8000\n\nError: ' + error.message);
            }

            if (attempt >= maxRetries) {
                setState('ocr');
            }
        }
    }
}

// Display Results
function displayResults(data) {
    // Update stats
    if (data.stats) {
        document.getElementById('statWords').textContent = data.stats.total_words || 0;
        document.getElementById('statSwitches').textContent = data.stats.switch_count || 0;

        const validPerc = data.stats.total_words > 0
            ? Math.round((data.stats.valid_words / data.stats.total_words) * 100)
            : 0;
        document.getElementById('statValid').textContent = validPerc + '%';
    }

    // Display language flow with hover tooltips
    const flowContainer = document.getElementById('flowContainer');
    flowContainer.innerHTML = '';

    if (data.results && Array.isArray(data.results)) {
        data.results.forEach(res => {
            const item = document.createElement('div');
            // Use language for class, default to unknown
            const langClass = (res.language || 'unknown').toLowerCase();
            item.className = `flow-item ${langClass}`;

            // Display: Original word (User Request: Complete words must be shown)
            item.textContent = res.original_token || res.word;

            // Create tooltip content
            const tooltipContent = createTooltipContent(res);
            item.setAttribute('data-tooltip', tooltipContent);

            // Add hover event listeners for tooltip
            item.addEventListener('mouseenter', showTooltip);
            item.addEventListener('mouseleave', hideTooltip);

            flowContainer.appendChild(item);
        });
    }

    setState('results');
}

// Create tooltip content for word
function createTooltipContent(wordData) {
    let content = `<div class="tooltip-header">
        <span class="tooltip-word">${wordData.word}</span>
        <span class="tooltip-lang-badge ${wordData.language ? wordData.language.toLowerCase() : 'unknown'}">${wordData.language || 'Unknown'}</span>
    </div>`;

    // Root Info
    if (wordData.root && wordData.root !== wordData.word) {
        content += `<div class="tooltip-row">
            <span class="tooltip-label">Root:</span>
            <span class="tooltip-value highlight">${wordData.root}</span>
        </div>`;
    }

    // POS
    if (wordData.pos) {
        content += `<div class="tooltip-row">
            <span class="tooltip-label">POS:</span>
            <span class="tooltip-pos">${wordData.pos}</span>
        </div>`;
    }

    // Structure
    if (wordData.structure) {
        content += `<div class="tooltip-morphemes">${wordData.structure}</div>`;
    }

    // Definitions with language badges and source links
    if (wordData.definitions && wordData.definitions.length > 0) {
        content += `<div class="tooltip-section">
            <div class="tooltip-section-title">Definitions</div>
            <div class="tooltip-def-entries">`;

        wordData.definitions.forEach(def => {
            const langBadge = def.language ? `<span class="tooltip-lang-badge ${def.language.toLowerCase()}">${def.language}</span>` : '';
            const posTag = def.pos ? `<span class="tooltip-def-pos">${def.pos}</span>` : '';
            const sourceLink = def.source ? `<a href="${def.source}" target="_blank" class="tooltip-link">↗</a>` : '';

            content += `<div class="tooltip-def-entry">
                <div class="tooltip-def-header">
                    ${langBadge}
                    ${posTag}
                    ${sourceLink}
                </div>
                <div class="tooltip-def-text">${def.definition || 'No definition'}</div>
            </div>`;
        });

        content += `</div></div>`;
    }

    // Affixes - simplified to just show type and language
    if (wordData.affix_functions && wordData.affix_functions.length > 0) {
        content += `<div class="tooltip-section">
            <div class="tooltip-section-title">Affixes</div>
            <div class="tooltip-affix-list">`;

        wordData.affix_functions.forEach(affix => {
            const typeClass = affix.type || 'other';
            const lang = affix.language || 'Unknown';

            content += `<div class="tooltip-affix-item compact">
                <span class="tooltip-affix-form">${affix.affix}</span>
                <span class="tooltip-affix-type ${typeClass}">${affix.type}</span>
                <span class="tooltip-lang-badge ${lang.toLowerCase()}">${lang}</span>
            </div>`;
        });

        content += `</div></div>`;
    }

    return content;
}

// Helper to truncate text
function truncateText(text, maxLen) {
    if (!text) return '';
    if (text.length <= maxLen) return text;
    return text.substring(0, maxLen) + '...';
}

// Show tooltip
function showTooltip(event) {
    const item = event.currentTarget;
    const tooltipHTML = item.getAttribute('data-tooltip');

    // Remove any existing tooltip
    hideTooltip();

    // Create tooltip element
    const tooltip = document.createElement('div');
    tooltip.className = 'flow-tooltip';
    tooltip.innerHTML = tooltipHTML;
    tooltip.id = 'active-tooltip';

    // Append to body to ensure it floats above all other content
    document.body.appendChild(tooltip);

    // Position tooltip
    const rect = item.getBoundingClientRect();
    const tooltipRect = tooltip.getBoundingClientRect();

    let left = rect.left + (rect.width / 2) - (tooltipRect.width / 2);
    let top = rect.top - tooltipRect.height - 8;

    // Keep tooltip within viewport
    if (left < 5) left = 5;
    if (left + tooltipRect.width > window.innerWidth - 5) {
        left = window.innerWidth - tooltipRect.width - 5;
    }

    // Check top edge
    if (top < 5) {
        // Show below if not enough space above
        top = rect.bottom + 8;
        tooltip.classList.add('below');
    }

    // Check bottom edge (if showing below puts it offscreen)
    // or if showing above puts it offscreen?
    // Actually, if it was moved below, check if it fits
    if (top + tooltipRect.height > window.innerHeight) {
        // If it doesn't fit below either, prefer above but pinned to top
        if (rect.top - tooltipRect.height - 8 < 5) {
            top = 5; // Pin to top
        } else {
            top = rect.top - tooltipRect.height - 8; // Revert to above
        }
    }

    tooltip.style.left = left + 'px';
    tooltip.style.top = top + 'px';


}

// Hide tooltip
function hideTooltip() {
    const tooltip = document.getElementById('active-tooltip');
    if (tooltip) {
        tooltip.remove();
    }
}

// Open Full Results
function openFullResults() {
    // Store results in storage
    chrome.storage.local.set({ lastAnalysis: analysisResults }, () => {
        const text = elements.extractedTextArea.value;
        const query = text ? `?q=${encodeURIComponent(text)}` : '';
        chrome.tabs.create({ url: `http://localhost:8000/analyzer${query}` });
    });
}

// Handle Paste
async function handlePaste() {
    try {
        const text = await navigator.clipboard.readText();
        if (text) {
            elements.extractedTextArea.value = text;
            setState('ocr');
        } else {
            showError('Clipboard is empty');
        }
    } catch (err) {
        // Fallback for Firefox or if permission denied
        setState('ocr');
        elements.extractedTextArea.focus();
        // Maybe show a tooltip "Ctrl+V to paste"
    }
}
// Utility Functions
function updateStatus(message) {
    elements.statusText.textContent = message;
}

function showError(message) {
    // Create a more user-friendly error display
    const errorDiv = document.createElement('div');
    errorDiv.style.cssText = `
        position: fixed;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        background: white;
        padding: 1.5rem;
        border-radius: 12px;
        box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
        max-width: 320px;
        z-index: 10000;
        border: 2px solid #d63031;
    `;

    errorDiv.innerHTML = `
        <div style="color: #d63031; font-weight: 600; margin-bottom: 0.75rem; font-size: 1rem;">
            ⚠️ Error
        </div>
        <div style="color: #1a1a1a; line-height: 1.5; font-size: 0.875rem; white-space: pre-line;">
            ${message}
        </div>
        <button id="errorOkBtn" style="
            margin-top: 1rem;
            width: 100%;
            padding: 0.5rem;
            background: #1a1a1a;
            color: white;
            border: none;
            border-radius: 6px;
            cursor: pointer;
            font-weight: 500;
        ">OK</button>
    `;

    document.body.appendChild(errorDiv);

    document.getElementById('errorOkBtn').addEventListener('click', () => {
        errorDiv.remove();
    });

    // Auto-remove after 10 seconds
    setTimeout(() => {
        if (errorDiv.parentNode) {
            errorDiv.remove();
        }
    }, 10000);
}
