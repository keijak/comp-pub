import sys
import bisect

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, D = inm()
A = [ini() for _ in range(N)]
S = sorted(A)


def solve():
    for a in A:
        j = bisect.bisect_right(S, a - D)
        print(j)


solve()
