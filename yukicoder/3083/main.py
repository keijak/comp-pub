import sys
from datetime import date, timedelta

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def is_leap(y):
    if y % 400 == 0:
        return True
    if y % 100 == 0:
        return False
    return y % 4 == 0


def solve():
    y, n, d = inl()
    mid = n - d
    mad = 365 - d
    if is_leap(y + 1):
        mad = 366 - d
    mi = max(mid, 0)
    ma = min(mad, n)
    return mi, ma


print(*solve())
