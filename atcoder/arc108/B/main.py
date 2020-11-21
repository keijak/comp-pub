import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    s = ins()
    sk = []
    for c in s:
        sk.append(c)
        if len(sk) >= 3 and "".join(sk[-3:]) == "fox":
            sk.pop()
            sk.pop()
            sk.pop()
    return len(sk)


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
