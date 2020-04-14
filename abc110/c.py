from collections import defaultdict

s = input()
t = input()

def fprint(x):
    d = defaultdict(list)
    for i, c in enumerate(x):
        d[c].append(i)
    return sorted(d.values())

print('Yes' if fprint(s) == fprint(t) else 'No')
