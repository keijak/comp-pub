import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, c, k = inl()
    t = [ini() for _ in range(n)]
    t.sort()
    if c == 1:
        return n

    def check(x):
        cur = t[0]
        cnt = 1
        i = 1
        while i < n:
            if cnt == c or t[i] > cur + k:
                x -= 1
                if x < 0:
                    return False
                cnt = 0
                cur = t[i]
            cnt += 1
            i += 1
        return x > 0

    fv, tv = 0, n + 1
    while tv - fv > 1:
        mid = (tv + fv) // 2
        if check(mid):
            tv = mid
        else:
            fv = mid
    return tv


print(solve())
