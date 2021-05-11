import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b = [int(x) for x in sys.stdin.readline().split()]
    va, vb = [], []
    sa, sb = 0, 0
    for i in range(1, a + 1):
        va.append(i)
        sa += i
    for i in range(1, b + 1):
        vb.append(-i)
        sb += i
    if sa > sb:
        d = sa - sb
        vb[-1] -= d
    elif sa < sb:
        d = sb - sa
        va[-1] += d
    return va + vb


print(*solve())
