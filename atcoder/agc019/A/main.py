import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    q, h, s, d = inl()
    n = ini()

    x = min(q * 4, h * 2, s)
    y = min(x * 2, d)

    ans = y * (n // 2)
    if n % 2 == 1:
        ans += x

    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
