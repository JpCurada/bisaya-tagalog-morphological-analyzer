// OCR Configuration
const OCR_CONFIG = {
    apiKey: 'API_KEY', // Your personal OCR.space API key
    apiUrl: 'https://api.ocr.space/parse/image',
    timeout: 30000,
    maxRetries: 2,
    imageQuality: 0.60,
    maxImageSize: 1000 // Reduced to 1000 for max speed
};

// Preprocess image for better OCR accuracy and smaller size
async function preprocessImage(imageDataUrl) {
    return new Promise((resolve, reject) => {
        const img = new Image();

        img.onload = () => {
            const canvas = document.createElement('canvas');
            const ctx = canvas.getContext('2d', { willReadFrequently: true });

            // Calculate optimal size based on device pixel ratio and max size
            const devicePxRatio = window.devicePixelRatio || 1;
            let width = img.width;
            let height = img.height;

            // Resize if too large
            if (width > OCR_CONFIG.maxImageSize || height > OCR_CONFIG.maxImageSize) {
                const ratio = Math.min(OCR_CONFIG.maxImageSize / width, OCR_CONFIG.maxImageSize / height);
                width = Math.floor(width * ratio);
                height = Math.floor(height * ratio);
            }

            canvas.width = width;
            canvas.height = height;

            // Draw image
            ctx.drawImage(img, 0, 0, width, height);

            // Convert to JPEG for smaller payload (prevent timeouts)
            const compressedDataUrl = canvas.toDataURL('image/jpeg', OCR_CONFIG.imageQuality);

            // Log for debugging
            console.log(`Image processed: ${imageDataUrl.length} -> ${compressedDataUrl.length} chars`);
            resolve(compressedDataUrl);
        };

        img.onerror = reject;
        img.src = imageDataUrl;
    });
}

// Perform OCR using OCR.space API
async function performOCRWithAPI(imageDataUrl) {
    const formData = new FormData();

    // Convert data URL to blob
    const base64Data = imageDataUrl.split(',')[1];
    // Detect format or default to jpeg if we just compressed it
    const isJpeg = imageDataUrl.startsWith('data:image/jpeg');
    const mimeType = isJpeg ? 'image/jpeg' : 'image/png';

    formData.append('base64Image', `data:${mimeType};base64,${base64Data}`);
    formData.append('apikey', OCR_CONFIG.apiKey);
    formData.append('language', 'eng');
    formData.append('isOverlayRequired', 'false');
    formData.append('detectOrientation', 'true');
    formData.append('scale', 'true');
    formData.append('OCREngine', '1'); // Engine 1 is faster (Legacy)

    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), OCR_CONFIG.timeout);

    try {
        const response = await fetch(OCR_CONFIG.apiUrl, {
            method: 'POST',
            body: formData,
            signal: controller.signal
        });

        clearTimeout(timeoutId);

        if (!response.ok) {
            throw new Error(`OCR API error: ${response.status}`);
        }

        const result = await response.json();

        if (result.IsErroredOnProcessing) {
            throw new Error(result.ErrorMessage || 'OCR processing failed');
        }

        if (!result.ParsedResults || result.ParsedResults.length === 0) {
            throw new Error('No text found in image');
        }

        return result.ParsedResults[0].ParsedText.trim();

    } catch (error) {
        clearTimeout(timeoutId);
        // Handle timeout specifically
        if (error.name === 'AbortError' || error.name === 'DOMException') {
            throw new Error('OCR request timed out. Please try a smaller area or check your internet connection.');
        }
        throw error;
    }
}

// Perform OCR with retry logic
async function performOCR(imageDataUrl, statusCallback) {
    let attempt = 0;

    while (attempt < OCR_CONFIG.maxRetries) {
        try {
            attempt++;
            if (statusCallback) {
                statusCallback(`Performing OCR... (Attempt ${attempt}/${OCR_CONFIG.maxRetries})`);
            }

            const processedImage = await preprocessImage(imageDataUrl);
            const text = await performOCRWithAPI(processedImage);

            console.log(`OCR completed successfully`);
            return text;

        } catch (error) {
            console.error(`OCR error (attempt ${attempt}):`, error);

            if (attempt < OCR_CONFIG.maxRetries) {
                if (statusCallback) {
                    statusCallback('OCR failed, retrying...');
                }
                await new Promise(resolve => setTimeout(resolve, 1000));
            } else {
                // If it's the last attempt, rethrow nicely usually, but here we throw to caller
                // Normalize error message if possible
                if (error.message) {
                    throw error;
                } else {
                    throw new Error('OCR failed for unknown reason.');
                }
            }
        }
    }
}
