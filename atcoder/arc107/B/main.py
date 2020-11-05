import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, k = inl()
    count = [0] * (2 * n + 1)
    for z in range(2, 2 * n + 1):
        if z - 1 <= n:
            count[z] = z - 1
        elif z - n <= n:
            count[z] = 2 * n - z + 1

    ans = 0
    for x in range(2, 2 * n + 1):
        y = x - k
        if y < 2 or y > 2 * n:
            continue
        ans += count[x] * count[y]
    return ans


print(solve())
