import requests
from lxml import html
import json
import re
import time
import os
import concurrent.futures

# Constants
BASE_URL = "https://en.wiktionary.org"
USER_AGENT = "BisayaTagalogScraper/2.0 (Contact: user@example.com)"
HEADERS = {'User-Agent': USER_AGENT}
DATA_DIR = "data_v2"

CATEGORIES = {
    "prefix_table.json": [
        ("https://en.wiktionary.org/wiki/Category:Tagalog_prefixes", "Tagalog"),
        ("https://en.wiktionary.org/wiki/Category:Cebuano_prefixes", "Cebuano")
    ],
    "suffix_table.json": [
        ("https://en.wiktionary.org/wiki/Category:Tagalog_suffixes", "Tagalog"),
        ("https://en.wiktionary.org/wiki/Category:Cebuano_suffixes", "Cebuano")
    ],
    "infix_table.json": [
        ("https://en.wiktionary.org/wiki/Category:Tagalog_infixes", "Tagalog"),
        ("https://en.wiktionary.org/wiki/Category:Cebuano_infixes", "Cebuano")
    ],
    "circumfix_table.json": [
        ("https://en.wiktionary.org/wiki/Category:Tagalog_circumfixes", "Tagalog"),
        ("https://en.wiktionary.org/wiki/Category:Cebuano_circumfixes", "Cebuano")
    ]
}

def clean_text(text):
    if not text: return ""
    return re.sub(r'\s+', ' ', text).strip()

def get_tree(url):
    try:
        time.sleep(0.1) 
        response = requests.get(url, headers=HEADERS, timeout=10)
        response.raise_for_status()
        return html.fromstring(response.content)
    except Exception as e:
        print(f"Error fetching {url}: {e}")
        return None

def extract_data_lxml(tree, language):
    # Locate the Language Header
    # XPath: //h2[span[@id='Language']]
    # OR //h2[text()='Language'] (but wiki uses spans usually)
    
    # We create a specific xpath for the header
    # Correct XPath to select H2 nodes that contain the language name, excluding TOC
    lang_header = tree.xpath(f"//h2[span[@id='{language}']] | //h2[contains(., '{language}') and not(ancestor::div[@id='toc'])]")
    
    if not lang_header:
        # Retry with just ID search if span approach fails (sometimes IDs are on h2 directly)
        lang_header = tree.xpath(f"//h2[@id='{language}']")
        if not lang_header:
            return None
    
    header_node = lang_header[0] # Take the first match (usually correct)
    
    # Check if header is wrapped in mw-heading (new MediaWiki output)
    parent = header_node.getparent()
    is_wrapped = parent is not None and 'mw-heading' in (parent.get('class') or '')
    
    if is_wrapped:
        # Traverse from the wrapper div
        current = parent.getnext()
    else:
        # Traverse from the h2 itself
        current = header_node.getnext()

    nodes = []
    while current is not None:
        # Stop condition: Next Language Header
        # If wrapped, it will be a div.mw-heading2 (or h2 if mixed?)
        # Generally, look for h2 or div with mw-heading2
        
        if current.tag == 'h2':
            break
        if current.tag == 'div' and 'mw-heading' in (current.get('class') or ''):
            # Check level. If it is heading2, stop.
            if 'mw-heading2' in current.get('class'):
                break
        
        nodes.append(current)
        current = current.getnext()
        
    # Helper to find text content within our collected nodes based on sub-headers
    # State machine approach
    section_data = {
        "etymology": [],
        "pronunciation": [],
        "syllabification": [],
        "function": [],
        "derived_terms": []
    }
    
    current_section = "etymology" # Default start, often Etymology is first without header
    
    for node in nodes:
        # Check for Headers
        if node.tag in ['h3', 'h4', 'h5']:
            header_text = node.text_content().strip().lower()
            
            if "etymology" in header_text:
                current_section = "etymology"
            elif "pronunciation" in header_text:
                current_section = "pronunciation"
            elif "derived terms" in header_text:
                current_section = "derived_terms"
            elif "definition" in header_text or "verb" in header_text or "noun" in header_text or "adjective" in header_text or "affix" in header_text or "prefix" in header_text or "suffix" in header_text or "infix" in header_text:
                # This matches Part of Speech headers, which contain the Definition
                current_section = "function"
            else:
                current_section = "other" # Unknown section
            continue
            
        # Content Extraction
        if current_section in section_data:
            # Special check for etytree div
            if node.tag == 'div' and 'etytree' in (node.get('class') or ''):
                 # This is definitely etymology, even if current_section mismatched (unlikely)
                 section_data["etymology"].append(clean_text(node.text_content()))
                 continue

            # Capture based on tags
            if node.tag in ['p', 'ul', 'ol', 'div', 'dl', 'table']:
                 text_content = ""
                 if node.tag in ['ul', 'ol']:
                     items = [clean_text(li.text_content()) for li in node.xpath('.//li')]
                     text_content = "\n".join(items)
                 else:
                     text_content = clean_text(node.text_content())
                 
                 if text_content:
                     section_data[current_section].append(text_content)

    # Etymology
    etymology = section_data["etymology"]
    
    # Pronunciation
    pronunciation_list = section_data["pronunciation"]
    ipa_text = None
    for item in pronunciation_list:
        if "IPA" in item:
            ipa_text = item
            break
    if not ipa_text and pronunciation_list:
        ipa_text = pronunciation_list[0] if pronunciation_list else None
        
    # Syllabification
    syllabification = []
    for item in pronunciation_list:
        if "Syllabification" in item:
            syllabification.append(item.replace("Syllabification", "").replace(":", "").strip())
    
    # Function/Meaning
    # Filter function data to extract definitions (numbered lists usually)
    # The 'function' bucket captures everything in POS section. We want lines that look like definitions.
    # But usually extract all is fine.
    definitions = section_data["function"]

    # Derived terms
    derived_terms = section_data["derived_terms"]
    
    # Data construction
    data = {
        "language": language,
        "etymology": "\n".join(etymology),
        "pronunciation": ipa_text,
        "syllabification": "\n".join(syllabification),
        "function": "\n".join(definitions),
        "derived_terms": derived_terms
    }
    
    return data

def process_affix_page(affix, url, language):
    tree = get_tree(url)
    if tree is None: return (affix, None)

    entry = extract_data_lxml(tree, language)
    if not entry:
        print(f"      Warning: Language {language} not found in {affix}")
        return (affix, None)
        
    print(f"    Scraped {affix} ({language})")
    return (affix, entry)

def scrape_category_page(url, language):
    tree = get_tree(url)
    if tree is None: return []

    # Get mw-pages
    # XPath: //div[@id='mw-pages']//a
    links = []
    
    # Anchor tags inside mw-pages div
    # Exclude those that are subcategories or next/prev links if inside
    # Actually Wiktionary categories are clean.
    
    link_nodes = tree.xpath("//div[@id='mw-pages']//div[@class='mw-content-ltr']//a")
    if not link_nodes:
        # Fallback
        link_nodes = tree.xpath("//div[@id='mw-pages']//a")
        
    for link in link_nodes:
        href = link.get('href')
        text = link.text_content()
        
        if href and not href.startswith('#'):
             # Basic Filters
            if 'Category:' in href or 'Wiktionary:' in href or 'Template:' in href or 'Help:' in href or 'Special:' in href: continue
             
            full_url = BASE_URL + href
            links.append((text, full_url))
    
    # Pagination
    # XPath for "next page": //a[contains(text(), 'next page')]
    next_link = tree.xpath("//a[contains(text(), 'next page')]")
    if next_link:
        print(f"  Found next page for {language}, recursing...")
        next_href = next_link[0].get('href')
        links.extend(scrape_category_page(BASE_URL + next_href, language))
        
    return links

def run_scraper():
    if not os.path.exists(DATA_DIR):
        os.makedirs(DATA_DIR)
        
    for filename, categories in CATEGORIES.items():
        print(f"Processing {filename}...")
        output_path = os.path.join(DATA_DIR, filename)
        
        compiled_data = {} 
        all_tasks = []
        
        for cat_url, lang in categories:
            print(f"  Fetching category: {lang} from {cat_url}")
            links = scrape_category_page(cat_url, lang)
            print(f"  Found {len(links)} affixes for {lang}.")
            for affix, url in links:
                all_tasks.append((affix, url, lang))
        
        print(f"  Starting parallel scrape of {len(all_tasks)} items...")
        with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
            future_to_affix = {executor.submit(process_affix_page, affix, url, lang): affix for affix, url, lang in all_tasks}
            
            for future in concurrent.futures.as_completed(future_to_affix):
                affix_name = future_to_affix[future]
                try:
                    result_affix, entry = future.result()
                    if entry:
                        if result_affix not in compiled_data:
                            compiled_data[result_affix] = []
                        compiled_data[result_affix].append(entry)
                except Exception as exc:
                    print(f'{affix_name} generated an exception: {exc}')
        
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(compiled_data, f, ensure_ascii=False, indent=2)
        print(f"Saved {filename} with {len(compiled_data)} entries.")

if __name__ == "__main__":
    run_scraper()
