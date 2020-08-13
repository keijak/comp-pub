import sys
import bisect

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    s = [ini() for _ in range(n)]
    s.sort()
    maxs = max(s)

    numpos = 0
    for x in s:
        if x > 0:
            numpos += 1

    def check(x, k):
        if x <= 0:
            return numpos <= k
        j = bisect.bisect_left(s, x)
        return n - j <= k

    def calc(k):
        ok = maxs + 1
        ng = -1
        while ok - ng > 1:
            mid = (ok + ng) // 2
            if check(mid, k):
                ok = mid
            else:
                ng = mid
        return ok

    q = ini()
    K = [ini() for _ in range(q)]
    for k in K:
        print(calc(k))


solve()
