import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, k, s = inl()
    x = 10 ** 9
    if x == s:
        x -= 1
    a = [None] * n
    for i in range(k):
        a[i] = s
    for i in range(k, n):
        a[i] = x
    return a


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(*solve())
