/**
 * Bistag Landing Page - Interactive Elements
 */

// Smooth scroll for anchor links
function initSmoothScroll() {
    DOM.qsa('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            e.preventDefault();
            const target = DOM.qs(this.getAttribute('href'));
            if (target) {
                DOM.scrollTo(target);
            }
        });
    });
}

// Initialize animations on scroll
function initScrollAnimations() {
    // Animate feature cards
    const featureCards = DOM.qsa('.feature-card');
    if (featureCards.length) {
        AnimationObserver.observe(featureCards);
    }

    // Animate steps
    const steps = DOM.qsa('.step');
    if (steps.length) {
        AnimationObserver.observe(steps);
    }
}

// Initialize landing page
function initLanding() {
    initSmoothScroll();
    initScrollAnimations();
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initLanding);
} else {
    initLanding();
}
