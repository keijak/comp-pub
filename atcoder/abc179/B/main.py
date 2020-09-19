import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    c = 0
    for i in range(n):
        x, y = inl()
        if x == y:
            c += 1
        else:
            c = 0
        if c == 3:
            return True
    return False


print("Yes" if solve() else "No")
