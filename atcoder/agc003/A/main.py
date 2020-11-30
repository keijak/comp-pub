import sys
from collections import defaultdict

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    d = defaultdict(lambda: False)
    for c in s:
        d[c] = True
    if d["N"] ^ d["S"]:
        return False
    if d["E"] ^ d["W"]:
        return False
    return True


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("Yes" if solve() else "No")
