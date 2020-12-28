import sys
from collections import Counter

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    counter = Counter(s)
    x, y = 0, 0
    for k, v in counter.items():
        if v % 2 == 1:
            x += 1
            y += (v - 1) // 2
        else:
            y += v // 2
    if x == 0:
        return 2 * y
    p = y // x
    return 1 + 2 * p


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())
