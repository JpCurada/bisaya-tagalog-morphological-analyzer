// Wrap in IIFE to avoid variable conflicts when script is injected multiple times
(function () {
    'use strict';

    // Check if already initialized
    if (window.bistagInitialized) {
        console.log('Bistag selection tool already initialized');
        return;
    }
    window.bistagInitialized = true;

    // Screenshot Selection Tool
    let isSelecting = false;
    let selectionOverlay = null;
    let selectionBox = null;
    let startX, startY;

    // Listen for messages from popup
    chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
        if (request.action === 'startSelection') {
            initializeSelection();
            sendResponse({ success: true });
        }
    });

    // Initialize Selection UI
    function initializeSelection() {
        if (isSelecting) return;

        isSelecting = true;

        // Create overlay
        selectionOverlay = document.createElement('div');
        selectionOverlay.id = 'bistag-selection-overlay';
        selectionOverlay.style.cssText = `
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.3);
            z-index: 999999;
            cursor: crosshair;
        `;

        // Create selection box
        selectionBox = document.createElement('div');
        selectionBox.id = 'bistag-selection-box';
        selectionBox.style.cssText = `
            position: fixed;
            border: 2px solid #6c5ce7;
            background: rgba(108, 92, 231, 0.1);
            z-index: 1000000;
            display: none;
            pointer-events: none;
        `;

        // Create instruction text
        const instruction = document.createElement('div');
        instruction.style.cssText = `
            position: fixed;
            top: 20px;
            left: 50%;
            transform: translateX(-50%);
            background: white;
            padding: 12px 24px;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
            font-family: -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 14px;
            color: #1a1a1a;
            z-index: 1000001;
        `;
        instruction.textContent = 'Click and drag to select area • Press ESC to cancel';

        selectionOverlay.appendChild(instruction);
        document.body.appendChild(selectionOverlay);
        document.body.appendChild(selectionBox);

        // Add event listeners
        selectionOverlay.addEventListener('mousedown', handleMouseDown);
        selectionOverlay.addEventListener('mousemove', handleMouseMove);
        selectionOverlay.addEventListener('mouseup', handleMouseUp);
        document.addEventListener('keydown', handleKeyDown);
    }

    // Mouse Event Handlers
    function handleMouseDown(e) {
        startX = e.clientX;
        startY = e.clientY;

        selectionBox.style.left = startX + 'px';
        selectionBox.style.top = startY + 'px';
        selectionBox.style.width = '0px';
        selectionBox.style.height = '0px';
        selectionBox.style.display = 'block';
    }

    function handleMouseMove(e) {
        if (!selectionBox.style.display || selectionBox.style.display === 'none') return;

        const currentX = e.clientX;
        const currentY = e.clientY;

        const width = Math.abs(currentX - startX);
        const height = Math.abs(currentY - startY);
        const left = Math.min(currentX, startX);
        const top = Math.min(currentY, startY);

        selectionBox.style.left = left + 'px';
        selectionBox.style.top = top + 'px';
        selectionBox.style.width = width + 'px';
        selectionBox.style.height = height + 'px';
    }

    async function handleMouseUp(e) {
        const rect = selectionBox.getBoundingClientRect();

        // Minimum selection size
        if (rect.width < 10 || rect.height < 10) {
            cleanup();
            return;
        }

        // Show processing indicator
        const instruction = selectionOverlay.querySelector('div');
        if (instruction) {
            instruction.textContent = 'Processing screenshot...';
            instruction.style.background = '#6c5ce7';
            instruction.style.color = 'white';
        }

        // Capture the selected area
        try {
            const dataUrl = await captureArea(rect);

            // Store in chrome.storage for popup to retrieve
            await chrome.storage.local.set({
                pendingScreenshot: dataUrl,
                screenshotTimestamp: Date.now()
            });

            // Notify background script
            chrome.runtime.sendMessage({
                action: 'screenshotCaptured',
                dataUrl: dataUrl
            });

            console.log('Screenshot captured and stored');
        } catch (error) {
            console.error('Screenshot capture error:', error);
            if (instruction) {
                instruction.textContent = 'Error: ' + error.message;
                instruction.style.background = '#d63031';
            }
            setTimeout(cleanup, 2000);
            return;
        }

        cleanup();
    }

    function handleKeyDown(e) {
        if (e.key === 'Escape') {
            chrome.runtime.sendMessage({ action: 'selectionCancelled' });
            cleanup();
        }
    }

    // Capture Selected Area
    async function captureArea(rect) {
        return new Promise((resolve, reject) => {
            chrome.runtime.sendMessage({
                action: 'captureVisibleTab',
                rect: {
                    x: rect.left,
                    y: rect.top,
                    width: rect.width,
                    height: rect.height
                }
            }, (response) => {
                if (response && response.dataUrl) {
                    // Crop the image to the selected area
                    cropImage(response.dataUrl, rect).then(resolve).catch(reject);
                } else {
                    reject(new Error('Failed to capture tab'));
                }
            });
        });
    }

    // Crop Image to Selection
    function cropImage(dataUrl, rect) {
        return new Promise((resolve, reject) => {
            const img = new Image();
            img.onload = () => {
                const canvas = document.createElement('canvas');
                const ctx = canvas.getContext('2d', { willReadFrequently: true });

                // The captured image already accounts for device pixel ratio
                // So we just need to crop the selected area
                canvas.width = rect.width;
                canvas.height = rect.height;

                // Draw the cropped portion
                const dpr = window.devicePixelRatio || 1;

                // Chrome captures at actual device pixels, while coordinates are CSS pixels
                // Thus we need to scale the source coordinates
                ctx.drawImage(
                    img,
                    rect.left * dpr,   // Source x
                    rect.top * dpr,    // Source y  
                    rect.width * dpr,  // Source width
                    rect.height * dpr, // Source height
                    0,                 // Dest x
                    0,                 // Dest y
                    rect.width,        // Dest width
                    rect.height        // Dest height
                );

                // Convert to JPEG for smaller file size
                resolve(canvas.toDataURL('image/jpeg', 0.92));
            };
            img.onerror = (err) => {
                console.error('Image load error:', err);
                reject(new Error('Failed to load captured image'));
            };
            img.src = dataUrl;
        });
    }

    // Cleanup
    function cleanup() {
        if (selectionOverlay) {
            selectionOverlay.remove();
            selectionOverlay = null;
        }
        if (selectionBox) {
            selectionBox.remove();
            selectionBox = null;
        }
        isSelecting = false;
        window.bistagInitialized = false; // Allow re-initialization
    }
})();
