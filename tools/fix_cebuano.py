import os

path = r'c:\Users\JP CURADA\bisaya-tagalog-morphological-analyzer\data\cebuano_dictionary.json'

print(f"Reading {path}...")
with open(path, 'r', encoding='utf-8') as f:
    text = f.read()

target = '},\nthose\n        "word"'
replacement = '},\n    {\n        "word"'

if target in text:
    print("Found corruption. Fixing...")
    fixed = text.replace(target, replacement)
    
    with open(path, 'w', encoding='utf-8') as f:
        f.write(fixed)
    print("File saved.")
else:
    print("Target string not found. File might vary from expectation or already fixed.")
