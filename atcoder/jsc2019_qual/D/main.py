import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    for i in range(n - 1):
        for j in range(n - 1 - i):
            v = i + 1 + j
            d = 0
            while True:
                if (i >> d & 1) != (v >> d & 1):
                    print(d + 1, end=" ")
                    break
                d += 1
        print()


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    solve()
