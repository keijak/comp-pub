import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()

prices = []
for i in range(N):
    a, b = inm()
    prices.append((a, b))
prices.sort()


def solve():
    cnt = 0
    amount = 0
    for a, b in prices:
        x = min(b, M - cnt)
        amount += x * a
        cnt += x
        if cnt >= M:
            break

    return amount


print(solve())
