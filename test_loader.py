#!/usr/bin/env python
"""Test the updated Python loader for new data format"""

import sys
import os

# Add project root to path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from src.python.loader import (
    load_bisaya_roots, 
    load_tagalog_roots, 
    load_shared_vocab,
    load_data
)

def main():
    print("=" * 50)
    print("Testing Python Loader for New Data Format")
    print("=" * 50)
    
    data_dir = "data"
    
    # Test root loading
    print("\n--- Loading Bisaya roots ---")
    bisaya = load_bisaya_roots(os.path.join(data_dir, "bisaya_roots.json"))
    print(f"Loaded {len(bisaya)} Bisaya roots")
    
    # Sample entry
    if 'palit' in bisaya:
        print(f"Sample - 'palit': {bisaya['palit']['definition'][:100]}...")
    
    print("\n--- Loading Tagalog roots ---")
    tagalog = load_tagalog_roots(os.path.join(data_dir, "tagalog_roots.json"))
    print(f"Loaded {len(tagalog)} Tagalog roots")
    
    if 'aba' in tagalog:
        print(f"Sample - 'aba': {tagalog['aba']['definition'][:100]}...")
    
    print("\n--- Loading shared vocab ---")
    shared = load_shared_vocab(os.path.join(data_dir, "shared_vocab.json"))
    print(f"Loaded {len(shared)} shared vocab items")
    
    if 'luto' in shared:
        print(f"Sample - 'luto': {shared['luto']}")
    
    # Test load_data for C initialization
    print("\n--- Testing load_data for C initialization ---")
    try:
        data = load_data(data_dir)
        print(f"prefix data length: {len(data['prefix'])} chars")
        print(f"suffix data length: {len(data['suffix'])} chars")
        print(f"bisaya data length: {len(data['bisaya'])} chars")
        print(f"tagalog data length: {len(data['tagalog'])} chars")
        print(f"shared data length: {len(data['shared'])} chars")
        print("\nAll load_data tests passed!")
    except Exception as e:
        print(f"Error in load_data: {e}")
        return 1
    
    print("\n" + "=" * 50)
    print("All Python loader tests passed!")
    print("=" * 50)
    return 0

if __name__ == "__main__":
    sys.exit(main())
