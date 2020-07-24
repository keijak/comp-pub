import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
a = inl()


def solve():
    mincnt = 100000
    for x in a:
        cnt = 0
        while x % 2 == 0:
            x //= 2
            cnt += 1
        mincnt = min(mincnt, cnt)

    return mincnt


print(solve())
