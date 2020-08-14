import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    N, X = inl()
    L = inl()
    cnt = 1
    d = 0
    for i in range(N):
        d += L[i]
        if d > X:
            break
        cnt += 1

    return cnt


print(solve())

