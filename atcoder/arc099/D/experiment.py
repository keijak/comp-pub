import sys
from fractions import Fraction
from collections import deque

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

L = 10 ** 7
K = ini()


def dsum(x):
    s = 0
    while x:
        s += x % 10
        x //= 10
    return s


def solve():
    minr = Fraction(10 ** 18, 1)
    ans = deque()
    for x in range(L, 0, -1):
        s = dsum(x)
        r = Fraction(x, s)
        if r <= minr:
            if x != L:
                # print(f"{x=}\t{s=}\t{float(r)}")
                ans.appendleft(x)
            minr = r
    for i, x in enumerate(ans):
        if i >= K:
            break
        print(x)
    # print(*ans, sep="\n")


try:
    import frosch

    frosch.hook()
except ImportError:
    pass

solve()
