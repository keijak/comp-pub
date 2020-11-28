import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    b = bin(n).count("1")
    if b <= 1:
        return (-1, -1, -1)
    c = n
    for i in range(32):
        if n & (1 << i):
            a = 1 << i
            b = n & ~a
            return (a, b, c)
    return (-1, -1, -1)


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(*solve())
