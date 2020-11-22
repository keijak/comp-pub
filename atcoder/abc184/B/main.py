import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, x = inl()
    s = ins()
    p = x
    for c in s:
        if c == "o":
            p += 1
        elif p == 0:
            continue
        else:
            p -= 1

    return p


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
