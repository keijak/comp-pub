import sys
from collections import defaultdict

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


n = ini()
a = inl()

if all(x == 0 for x in a):
    print(0)
    sys.exit()

b = [0]
for i in range(n):
    if a[i] != b[-1]:
        b.append(a[i])
if b[-1] != 0:
    b.append(0)
nb = len(b)

c = defaultdict(list)
keys = set()
for i in range(1, nb - 1):
    x = b[i]
    t = [x]
    t.append(b[i - 1])
    t.append(b[i + 1])
    if min(t) == x:
        c[x].append((False, i))
        keys.add(x)
    if max(t) == x:
        c[x].append((True, i))
        keys.add(x)

keys = sorted(keys)
num = 1
ans = 1
for x in keys:
    for maximal, j in c[x]:
        if maximal:
            num -= 1
        else:
            num += 1
    ans = max(ans, num)
print(ans)
