import sys
import bisect

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    N, K = inl()
    R = inl()
    R.sort(reverse=True)
    R = R[:K]
    cur = 0
    while R:
        r = R.pop()
        assert r >= cur
        cur = (cur + r) / 2
    return cur


print(solve())
