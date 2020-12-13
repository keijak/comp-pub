import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m, t = inl()
    bat = n
    now = 0
    for i in range(m):
        a, b = inl()
        # debug(f"{i=} {now=} {bat=} {a=} {b=}")
        assert now <= a
        d = a - now
        if bat <= d:
            return False
        bat -= d
        bat += b - a
        bat = min(bat, n)
        now = b
    if bat <= t - now:
        return False

    return True


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("Yes" if solve() else "No")
