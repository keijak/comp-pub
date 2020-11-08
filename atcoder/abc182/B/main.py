import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    a = inl()
    mx = 0
    ma = 2
    for x in range(2, 1001):
        cnt = 0
        for y in a:
            if y % x == 0:
                cnt += 1
        if mx < cnt:
            mx = cnt
            ma = x

    return ma


print(solve())
