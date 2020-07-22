import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n, m = inm()
a = inl()
c = Counter(a)


def solve():
    v, cnt = c.most_common(1)[0]
    if 2 * cnt > n:
        return v
    else:
        return "?"


print(solve())
