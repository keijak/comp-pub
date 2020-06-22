import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **dict(file=sys.stderr, **kw))

N = ini()
D = [ini() for _ in range(N)]


def solve():
    dmax = max(D)
    dsum = sum(D)
    dmin = max(dmax - (dsum - dmax), 0)
    return [dsum, dmin]


print(*solve(), sep="\n")
