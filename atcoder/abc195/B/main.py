import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, w = inl()
    w *= 1000
    if w < a:
        return None
    kb = (w + b - 1) // b
    rb = w % b
    xb = (rb + kb - 1) // kb
    if b - xb < a:
        return None

    ka = w // a
    ra = w % a
    xa = (ra + ka - 1) // ka
    if a + xa > b:
        return None
    return (kb, ka)


res = solve()
if res is None:
    print("UNSATISFIABLE")
else:
    print(*res)
