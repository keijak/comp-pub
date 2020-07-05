import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, A, B = inm()


def solve():
    if A > B:
        return 0
    if N == 1:
        return 0 if A != B else 1
    a = A * (N - 1) + B
    b = B * (N - 1) + A
    return b - a + 1


print(solve())
