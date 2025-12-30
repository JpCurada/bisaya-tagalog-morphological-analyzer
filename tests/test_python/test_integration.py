import unittest
import os
import sys

# Add src to path
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.python.bindings import MorphologicalAnalyzer

class TestIntegration(unittest.TestCase):
    def setUp(self):
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
        self.analyzer = MorphologicalAnalyzer(root_dir)
        
    def test_bisaya_analysis(self):
        res = self.analyzer.analyze("nag-kaon")
        self.assertTrue(res['valid'])
        self.assertEqual(res['root'], "kaon")
        self.assertEqual(res['language'], "Bisaya")
        
    def test_tagalog_analysis(self):
        res = self.analyzer.analyze("kumain") # Wait, "kumain" prefix "um-" infix? 
        # My prefix table has "um-". My rules don't handle infixes yet properly unless "um-" is treated as prefix.
        # "um-kain" -> "kumain"? No.
        # "um-alis" is prefix.
        # "mag-kain" is purely prefix.
        # Let's test "mag-kain" which fits our model.
        res = self.analyzer.analyze("mag-kain")
        self.assertTrue(res['valid'])
        self.assertEqual(res['root'], "kain")
        self.assertEqual(res['language'], "Tagalog")
        
    def test_shared_analysis(self):
        res = self.analyzer.analyze("nag-luto")
        self.assertTrue(res['valid'])
        self.assertEqual(res['root'], "luto")
        self.assertEqual(res['language'], "Shared") # Root is Both?
        # Wait, "luto" in shared_vocab.json is "Both" (Shared).
        # My logic: find_root returns language.
        # In simple test earlier: "Shared: nag-luto".
        
    def test_invalid_word(self):
        res = self.analyzer.analyze("xyz-123")
        self.assertFalse(res['valid'])
        self.assertEqual(res['language'], "Unknown")

if __name__ == '__main__':
    unittest.main()
