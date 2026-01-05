# Bistag Extension Changelog

## Version 1.1.0 (2025-12-31)

### Added
- **Image Preprocessing**: Automatically resize and optimize images based on device pixel ratio for better OCR accuracy
- **Retry Logic**: Automatic retry for both OCR and API calls (up to 2 retries)
- **Progress Indicators**: Visual progress bar and detailed status messages during processing
- **Timeout Handling**: 15-second timeout for API requests with automatic retry
- **Confidence Logging**: OCR confidence scores logged to console for debugging

### Improved
- **Error Messages**: More detailed and user-friendly error messages
- **OCR Accuracy**: Image preprocessing improves text detection quality
- **Reliability**: Retry logic handles temporary failures gracefully
- **User Feedback**: Better visual feedback during all processing stages

### Fixed
- Content script variable redeclaration error (wrapped in IIFE)
- Canvas performance warning (added willReadFrequently flag)
- Screenshot capture on restricted pages (added URL validation)

## Version 1.0.0 (2025-12-30)

### Initial Release
- Screenshot capture with drag-to-select
- OCR text extraction using Tesseract.js
- Morphological analysis integration
- Language flow visualization
- Statistics display
- Chrome extension manifest v3 support
