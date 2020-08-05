import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

E = set(inl())
B = ini()
L = set(inl())


def solve():
    d = E & L
    m = len(d)
    if m == 6:
        return 1
    if m == 5:
        b = list(L - E)[0]
        if b == B:
            return 2
        return 3
    elif len(d) < 3:
        return 0
    else:
        return 8 - len(d)


print(solve())
