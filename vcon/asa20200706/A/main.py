import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, K = inm()
T = [ini() for _ in range(N)]


def solve():
    a1 = T[0]
    a2 = T[1]
    for i in range(2, N):
        if a1 + a2 + T[i] < K:
            return i + 1
        a1 = a2
        a2 = T[i]

    return -1


print(solve())
