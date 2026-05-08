#!/usr/bin/env python3
import re
import sys
from pathlib import Path

if len(sys.argv) != 2:
    print("Usage: update_build.py stuff", file=sys.stderr)
    sys.exit(1)

path = Path(sys.argv[1])
if not path.exists():
    path.write_text("280\n")

text = path.read_text().strip()
if not text.isdigit():
    print(f"Error: version file does not contain an integer: {text}", file=sys.stderr)
    sys.exit(1)

old_build = int(text)
new_build = old_build + 1
path.write_text(f"{new_build}\n")
print(f"Updated build counter: {old_build} -> {new_build}")
