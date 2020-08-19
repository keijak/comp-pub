import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    a = inl()
    c = Counter(a)
    x, k = c.most_common(1)[0]
    if k > n // 2:
        return x

    return "?"


print(solve())
