import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

x, y = inm()


def solve():
    if 4 * x - y < 0:
        return False
    if (4 * x - y) % 2 == 1:
        return False

    a = (4 * x - y) // 2
    return x - a >= 0


print("Yes" if solve() else "No")
