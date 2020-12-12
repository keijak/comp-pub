import sys
from fractions import Fraction

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x, y = tuple(map(int, ins().split("/")))
    if 2 * x % y == 0:
        l = 2 * x // y - 1
        r = l + 2
    else:
        l = (2 * x + y - 1) // y - 1
        r = 2 * x // y + 1

    d = Fraction(x, y)
    found = False
    for n in range(min(l, 1), r + 1):
        m = (Fraction(n + 1, 2) - d) * n
        if m.denominator != 1:
            continue
        p = m.numerator
        if 1 <= p <= n:
            print(n, p)
            found = True
    if not found:
        print("Impossible")


if __name__ == "__main__":
    # try:
    #     import frosch

    #     frosch.hook()
    # except ImportError:
    #     pass
    solve()
