import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    a = inl()
    b = inl()
    if n < m:
        return False
    a.sort(reverse=True)
    b.sort(reverse=True)
    for i, x in enumerate(b):
        if x > a[i]:
            return False
    return True


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("YES" if solve() else "NO")
