import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    x, y = inl()
    if y == 0:
        return "ERROR"
    x *= 100
    x //= y
    z = x % 100
    w = x // 100
    return "{}.{:02}".format(w, z)


print(solve())
