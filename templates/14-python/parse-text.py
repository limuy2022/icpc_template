import re

key, value = input().split(":", 1)
match = re.fullmatch(r"\s*([+-]?[0-9]+)\s*", value)
if match:
    print(key.strip(), int(match[1]))
