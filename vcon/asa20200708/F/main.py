import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


N = ini()
magic = [inl() for _ in range(N)]
magic_a = [(a, b) for a, b in magic if a < b]
magic_b = [(b, a) for a, b in magic if a > b]
magic_c = [a for a, b in magic if a == b]


def solve():
    magic_a.sort()
    magic_b.sort()

    hmax = 0
    h = 0
    for (a, b) in magic_a:
        h += a
        if hmax < h:
            hmax = h
        h -= b

    if magic_c:
        cmax = max(magic_c)
        if hmax < h + cmax:
            hmax = h + cmax

    rmax = 0
    r = 0
    for (b, a) in magic_b:
        r += b
        if rmax < r:
            rmax = r
        r -= a

    delta = r - h
    if hmax < rmax - delta:
        hmax = rmax - delta

    return hmax


print(solve())
