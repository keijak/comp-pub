import sys
from fractions import Fraction
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = [Fraction(sys.stdin.readline()) for _ in range(N)]


def solve():
    nums = Counter()
    dens = Counter()
    # for i in range(N):
    #     A[i]
    return A[:3]


print(solve())
