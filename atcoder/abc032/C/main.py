import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, k = inl()
    s = [ini() for _ in range(n)]
    if 0 in s:
        return n
    ans = 0
    r, p = 0, 1
    for l in range(n):
        if r < l:
            r = l
            p = 1
        while r < n and p <= k:
            p *= s[r]
            r += 1
            if p <= k:
                ans = max(ans, r - l)
        p //= s[l]
    return ans


print(solve())
