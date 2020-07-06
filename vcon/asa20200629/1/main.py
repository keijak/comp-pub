import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

A, B = inm()


def solve():
    for x in range(200000):
        a = int(x * 0.08 + 1e-9)
        b = int(x * 0.10 + 1e-9)
        if a == A and b == B:
            return x

    return -1


print(solve())
