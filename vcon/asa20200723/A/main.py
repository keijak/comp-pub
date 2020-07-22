import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

ss, st = ins().split()
a, b = inm()
su = ins()


def solve():
    if su == ss:
        return (a - 1), b
    else:
        return a, (b - 1)


print(*solve())
