import sys
from itertools import accumulate
import bisect

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M, K = inm()
A = inl()
B = inl()


def solve():
    acum = accumulate(A)
    bcum = list(accumulate(B))
    ans = 0
    for i, b in enumerate(bcum):
        if b <= K:
            ans = i + 1
    for i, x in enumerate(acum):
        if x > K:
            break

        j = bisect.bisect_right(bcum, K - x)
        s = i + 1 + j
        if ans < s:
            ans = s

    return ans


print(solve())
