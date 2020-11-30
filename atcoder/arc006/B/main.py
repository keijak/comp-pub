import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    N, L = inl()
    amida = [None] * L
    for i in range(L):
        amida[i] = ins()
    goal = ins()
    g = goal.index("o") // 2
    debug(g)
    for i in range(L - 1, -1, -1):
        row = amida[i]
        for j, c in enumerate(row):
            if c == "-":
                x = (j - 1) // 2
                y = (j + 1) // 2
                if g == x:
                    g = y
                elif g == y:
                    g = x
    return g + 1


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
