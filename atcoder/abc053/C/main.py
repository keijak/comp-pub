import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x = ini()
    k = x // 11
    m = x % 11
    if m == 0:
        a = 0
    elif m <= 6:
        a = 1
    else:
        a = 2
    return 2 * k + a


print(solve())
