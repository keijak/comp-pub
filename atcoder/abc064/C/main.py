import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A = nl()


def solve():
    c = collections.Counter()
    cf = 0
    for a in A:
        if a < 400:
            c[0] += 1
        elif a < 800:
            c[1] += 1
        elif a < 1200:
            c[2] += 1
        elif a < 1600:
            c[3] += 1
        elif a < 2000:
            c[4] += 1
        elif a < 2400:
            c[5] += 1
        elif a < 2800:
            c[6] += 1
        elif a < 3200:
            c[7] += 1
        else:
            cf += 1
    mn = len(c)
    if mn == 0 and cf > 0:
        mn = 1
    mx = len(c) + cf
    print(mn, mx)


solve()
