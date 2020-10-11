import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    v = inl()
    total = sum(v)
    for s in range(1 << 4):
        eat = 0
        for i in range(4):
            if s & (1 << i):
                eat += v[i]
        if eat * 2 == total:
            return True
    return False


print("Yes" if solve() else "No")

