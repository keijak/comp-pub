import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x = ini()
    a = x // 500
    ans = a * 1000
    x %= 500
    b = x // 5
    ans += b * 5
    debug(a, b)
    return ans


print(solve())
