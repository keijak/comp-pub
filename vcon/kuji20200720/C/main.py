import sys
from itertools import accumulate

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
a = inl()


def solve():
    al = list(accumulate(a))
    ar = list(reversed(list(accumulate(a[::-1]))))
    ans = 1 << 60
    for i in range(1, n):
        r = abs(al[i - 1] - ar[i])
        ans = min(ans, r)

    return ans


print(solve())
