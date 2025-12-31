# Bistag Chrome Extension

## Setup Instructions

### 1. Download Tesseract.js

The extension uses Tesseract.js for OCR. Download it:

```bash
cd extension
mkdir lib
cd lib

# Download Tesseract.js (v4.0.0 or later)
# Visit: https://cdn.jsdelivr.net/npm/tesseract.js@4/dist/tesseract.min.js
# Save as: tesseract.min.js
```

Or use this direct link:
```
https://cdn.jsdelivr.net/npm/tesseract.js@4.0.0/dist/tesseract.min.js
```

### 2. Create Extension Icons

Create three icon sizes in the `icons/` folder:
- `icon16.png` (16x16)
- `icon48.png` (48x48)
- `icon128.png` (128x128)

You can use any image editor or online tool to create these.

### 3. Load Extension in Chrome

1. Open Chrome and go to `chrome://extensions/`
2. Enable "Developer mode" (toggle in top right)
3. Click "Load unpacked"
4. Select the `extension` folder
5. The Bistag extension should now appear in your extensions list

### 4. Start the Bistag Server

Make sure the Bistag server is running:
```bash
python src/python/server.py
```

The server should be accessible at `http://localhost:8000`

## How to Use

1. **Click the Extension Icon**: Opens the Bistag popup
2. **Click "Capture Screenshot"**: Your cursor changes to crosshair mode
3. **Select Area**: Click and drag to select the text area you want to analyze
4. **OCR Processing**: The extension extracts text from the screenshot
5. **Analysis**: Text is automatically sent to the Bistag analyzer (if auto-analyze is enabled)
6. **View Results**: See language flow and statistics in the popup
7. **View Full Details**: Click to open the full web interface with detailed results

## Features

- **Screenshot Capture**: Drag-to-select any area on a webpage
- **OCR Text Extraction**: Powered by OCR.space API for accurate text recognition
- **Morphological Analysis**: Analyze Bisaya-Tagalog code-switching patterns
- **Language Flow Visualization**: See language transitions in your text
- **Statistics**: View word counts, switch counts, and validity percentages

## Troubleshooting

### Extension Not Working
- Make sure the Bistag server is running at `http://localhost:8000`
- Check the browser console for errors (F12 → Console)
- Reload the extension in `chrome://extensions/`

### OCR Not Detecting Text
- Ensure the screenshot has clear, readable text
- Try capturing a larger area
- Ensure you have a valid OCR.space API key configured in the extension.

### API Connection Failed
- Verify the server is running: `python src/python/server.py`
- Check the API URL in extension settings
- Ensure no firewall is blocking localhost connections

## File Structure

```
extension/
├── manifest.json       # Extension configuration
├── popup.html         # Extension popup UI
├── popup.css          # Popup styles
├── popup.js           # Popup logic
├── content.js         # Screenshot selection
├── content.css        # Content script styles
├── background.js      # Background service worker
├── lib/
│   └── tesseract.min.js  # OCR library
└── icons/
    ├── icon16.png
    ├── icon48.png
    └── icon128.png
```

## Development

To modify the extension:
1. Make changes to the files
2. Go to `chrome://extensions/`
3. Click the reload icon on the Bistag extension
4. Test your changes

## Privacy

This extension:
- Only captures screenshots when you explicitly click "Capture Screenshot"
- Processes OCR locally in your browser
- Only sends extracted text to your local Bistag server
- Does not collect or transmit any data to external servers
