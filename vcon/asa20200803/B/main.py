import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

X = ins()


def solve():
    i = 0
    n = len(X)
    while i < n:
        if X[i : i + 2] == "ch":
            i += 2
        elif X[i] in ["o", "k", "u"]:
            i += 1
        else:
            return False
    return True


print("YES" if solve() else "NO")
