import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x = inl()
    y = sorted(x)
    m = y[1]
    if x[0] == m:
        return "A"
    elif x[1] == m:
        return "B"
    else:
        return "C"


print(solve())
