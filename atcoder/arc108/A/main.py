import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s, p = inl()

    x = 1
    while x * x <= p:
        if p % x != 0:
            x += 1
            continue
        y = p // x
        if x == s - y:
            return True
        x += 1
    return False


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("Yes" if solve() else "No")
