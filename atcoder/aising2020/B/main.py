import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()


def solve():
    cnt = 0
    for i in range(N):
        if (i + 1) % 2 == 1 and A[i] % 2 == 1:
            cnt += 1

    return cnt


print(solve())
