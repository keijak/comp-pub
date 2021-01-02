import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    ss = [ins() for _ in range(n)]
    sp = set()
    sn = set()
    for s in ss:
        if s.startswith("!"):
            sn.add(s[1:])
        else:
            sp.add(s)
    for s in sp:
        if s in sn:
            return s
    return "satisfiable"


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())
