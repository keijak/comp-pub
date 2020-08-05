import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()


def solve():
    c = Counter()
    for a in A:
        c[a - 1] += 1
        c[a] += 1
        c[a + 1] += 1

    x, k = c.most_common()[0]

    return k


print(solve())
