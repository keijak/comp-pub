import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    A, B, C = inl()
    if A % 2 == 0 or B % 2 == 0 or C % 2 == 0:
        return 0
    x = sorted([A, B, C])

    return x[0] * x[1]


print(solve())
