// OCR Configuration
const OCR_CONFIG = {
    apiKey: 'K87899142388957', // Free OCR.space API key
    apiUrl: 'https://api.ocr.space/parse/image',
    timeout: 30000,
    maxRetries: 2
};

// Preprocess image for better OCR accuracy
async function preprocessImage(imageDataUrl) {
    return new Promise((resolve, reject) => {
        const img = new Image();

        img.onload = () => {
            const canvas = document.createElement('canvas');
            const ctx = canvas.getContext('2d', { willReadFrequently: true });

            // Calculate optimal size based on device pixel ratio
            const devicePxRatio = window.devicePixelRatio || 1;
            const scaleValue = 1 / devicePxRatio;

            const wantedWidth = img.width * scaleValue;
            const wantedHeight = img.height * scaleValue;

            // Set canvas dimensions
            canvas.width = wantedWidth;
            canvas.height = wantedHeight;

            // Draw resized image
            ctx.drawImage(img, 0, 0, wantedWidth, wantedHeight);

            // Convert back to data URL
            resolve(canvas.toDataURL('image/png'));
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
    formData.append('base64Image', `data:image/png;base64,${base64Data}`);
    formData.append('apikey', OCR_CONFIG.apiKey);
    formData.append('language', 'eng');
    formData.append('isOverlayRequired', 'false');
    formData.append('detectOrientation', 'true');
    formData.append('scale', 'true');
    formData.append('OCREngine', '2'); // Engine 2 is better for general text

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

    } finally {
        clearTimeout(timeoutId);
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

            // Preprocess image for better accuracy
            const processedImage = await preprocessImage(imageDataUrl);

            // Perform OCR using API
            const text = await performOCRWithAPI(processedImage);

            console.log(`OCR completed successfully`);

            if (!text) {
                if (attempt < OCR_CONFIG.maxRetries) {
                    if (statusCallback) {
                        statusCallback('No text detected, retrying...');
                    }
                    await new Promise(resolve => setTimeout(resolve, 1000));
                    continue;
                }
                throw new Error('No text detected in the screenshot. Please try again with clearer text.');
            }

            return text; // Return the extracted text

        } catch (error) {
            console.error(`OCR error (attempt ${attempt}):`, error);

            if (attempt < OCR_CONFIG.maxRetries) {
                if (statusCallback) {
                    statusCallback('OCR failed, retrying...');
                }
                await new Promise(resolve => setTimeout(resolve, 1000));
            } else {
                throw error; // Throw error to be handled by caller
            }
        }
    }
}
