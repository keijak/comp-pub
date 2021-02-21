import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    for i, c in enumerate(s):
        if i % 2 == 0:
            if not c.islower():
                return False
        else:
            if c.islower():
                return False
    return True


print(["No", "Yes"][solve()])

