import os
import re
from collections import defaultdict

# Configuration
DIRECTORY = os.getcwd()  # Replace with your folder path
ADD_ITEM_REGEX = re.compile(
    r'AddItem\s*\(\s*"([^"]+)"\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*(null|\{[^}]*\})\s*)?(?:,\s*(null|\{[^}]*\})\s*)?\)\s*;'
)

# Step 1: Parse all items and store their data
all_items = {}
variants_map = defaultdict(list)
item_locations = {}  # Map item -> (filename, full match)

for root, _, files in os.walk(DIRECTORY):
    for filename in files:
        if not filename.endswith(".c"):
            continue
        filepath = os.path.join(root, filename)
        with open(filepath, "r") as f:
            content = f.read()

        for match in ADD_ITEM_REGEX.finditer(content):
            itemname = match.group(1).lower()
            sellprice = int(match.group(2))
            buyprice = int(match.group(3))
            variant_raw = match.group(7)

            if variant_raw and variant_raw != "null":
                variants = [v.strip().strip('"').lower() for v in variant_raw.strip("{}").split(",") if v.strip()]
                for variant in variants:
                    variants_map[variant].append(itemname)

            all_items[itemname] = {
                "sellprice": sellprice,
                "buyprice": buyprice,
                "line": match.group(0),
                "file": filepath,
                "span": match.span()
            }
            item_locations[itemname] = (filepath, match)

# Step 2: Identify and remove variants with matching prices
to_remove = defaultdict(list)

for variant, parents in variants_map.items():
    if variant not in all_items:
        continue
    v_data = all_items[variant]
    for parent in parents:
        if parent not in all_items:
            continue
        p_data = all_items[parent]
        if v_data["sellprice"] == p_data["sellprice"] and v_data["buyprice"] == p_data["buyprice"]:
            to_remove[v_data["file"]].append(v_data["line"])
            break  # Only need one parent to match

# Step 3: Modify the files
for file_path, lines_to_remove in to_remove.items():
    with open(file_path, "r") as f:
        lines = f.readlines()
    
    with open(file_path, "w") as f:
        for line in lines:
            if not any(line.strip() == l.strip() for l in lines_to_remove):
                f.write(line)

print("Done. Removed", sum(len(v) for v in to_remove.values()), "items across", len(to_remove), "files.")
