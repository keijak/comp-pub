import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n, m, k = inm()


def solve():
    for x in range(n + 1):
        for y in range(m + 1):
            a = m * x + n * y - 2 * x * y
            if a == k:
                return True
    return False


print("Yes" if solve() else "No")
