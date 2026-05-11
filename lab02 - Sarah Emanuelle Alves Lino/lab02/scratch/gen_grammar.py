import sys

def format_grammar():
    with open('grammar.txt', 'r') as f:
        lines = f.read().splitlines()

    conf_lines = []
    js_lines = []

    for line in lines:
        if not line.strip():
            continue
        parts = line.split('->')
        if len(parts) != 2:
            continue
        left = parts[0].strip()
        rights = parts[1].split('|')
        
        for right in rights:
            right = right.strip()
            
            # js format
            js_right = right
            js_lines.append(f"{left} -> {js_right}")
            
            # conf format
            if right == "''":
                conf_right = ""
            else:
                conf_right = right
            # note: -> must have space before and after
            if conf_right:
                conf_lines.append(f"{left} -> {conf_right}")
            else:
                conf_lines.append(f"{left} -> ")

    with open('../gramatica.conf', 'w') as f:
        f.write('\n'.join(conf_lines) + '\n')
        
    with open('js_grammar.txt', 'w') as f:
        f.write('\n'.join(js_lines) + '\n')

if __name__ == '__main__':
    format_grammar()
