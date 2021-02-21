import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x = ins()
    n = len(x)
    d = 0
    for c in x:
        y = ord(c) - ord("0")
        d = max(d, y)
    m = ini()

    if n == 1:
        if int(x) <= m:
            return 1
        return 0

    def check(z):
        bs = 1
        num = 0
        for i in range(n):
            y = ord(x[n - 1 - i]) - ord("0")
            num += y * bs
            if num > m:
                return False
            bs *= z
        return num <= m

    if not check(d + 1):
        return 0

    tv, fv = (d + 1), (10 ** 18 + 5)
    while fv - tv > 1:
        mid = (fv + tv) // 2
        if check(mid):
            tv = mid
        else:
            fv = mid
    return tv - d


print(solve())
