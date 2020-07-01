import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, Y = inm()
FALSE = (-1, -1, -1)


def solve():
    if Y % 1000 != 0:
        return FALSE
    y = Y // 1000
    for c in range(N + 1):
        b4 = y - 9 * c - N
        if b4 < 0 or b4 > 4 * N or b4 % 4 != 0:
            continue
        b = b4 // 4
        a = N - b - c
        if a < 0 or a > N:
            continue
        return c, b, a
    return FALSE


print(*solve())
