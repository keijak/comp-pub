import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    a = [0] * 6
    for i in range(n):
        mx, mn = map(float, ins().split())
        if mx >= 35:
            a[0] += 1
        if 30 <= mx < 35:
            a[1] += 1
        if 25 <= mx < 30:
            a[2] += 1
        if 25 <= mn:
            a[3] += 1
        if mn < 0 and 0 <= mx:
            a[4] += 1
        if mx < 0:
            a[5] += 1

    return a


print(*solve())
