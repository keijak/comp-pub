import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    sc = Counter(s)
    counts = [sc["a"], sc["b"], sc["c"]]
    return max(counts) - min(counts) <= 1


print("YES" if solve() else "NO")
