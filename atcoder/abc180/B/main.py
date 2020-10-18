import sys
import math

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    x = inl()
    m = 0
    e = 0
    t = abs(x[0])
    for y in x:
        m += abs(y)
        e += y * y
        t = max(t, abs(y))
    e = math.sqrt(e)
    return (m, e, t)


print(*solve(), sep="\n")
