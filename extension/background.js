// Background Service Worker

// Handle messages from content script
chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    if (request.action === 'captureVisibleTab') {
        chrome.tabs.captureVisibleTab(
            null,
            { format: 'png' },
            (dataUrl) => {
                sendResponse({ dataUrl: dataUrl });
            }
        );
        return true; // Keep message channel open for async response
    }

    if (request.action === 'screenshotCaptured') {
        // Open popup to process the screenshot
        chrome.action.openPopup().catch(err => {
            console.log('Could not open popup automatically:', err);
            // Popup will check storage when user opens it manually
        });
    }
});

// Handle extension installation
chrome.runtime.onInstalled.addListener(() => {
    console.log('Bistag extension installed');

    // Set default settings
    chrome.storage.sync.set({
        autoAnalyze: true,
        apiUrl: 'http://localhost:8000'
    });
});
