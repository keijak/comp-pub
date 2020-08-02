import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, D = inl()


def solve():
    cnt = 0
    for i in range(N):
        x, y = inl()
        if x * x + y * y <= D * D:
            cnt += 1
    return cnt


print(solve())
