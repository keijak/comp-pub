import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

C = [inl() for _ in range(3)]


def solve():
    a10 = C[1][0] - C[0][0]
    if C[1][1] - C[0][1] != a10 or C[1][2] - C[0][2] != a10:
        return False
    a21 = C[2][0] - C[1][0]
    if C[2][1] - C[1][1] != a21 or C[2][2] - C[1][2] != a21:
        return False
    b10 = C[0][1] - C[0][0]
    if C[1][1] - C[1][0] != b10 or C[2][1] - C[2][0] != b10:
        return False
    b21 = C[0][2] - C[0][1]
    if C[1][2] - C[1][1] != b21 or C[2][2] - C[2][1] != b21:
        return False
    return True


print("Yes" if solve() else "No")
