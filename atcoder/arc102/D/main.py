import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

L = ini()


def solve():
    k = 1
    g = []
    while True:
        weight = 2 ** (k - 1)
        if 2 * weight >= L:
            break
        g.append((k, k - 1, 0))
        g.append((k, k - 1, weight))
        k += 1

    debug(f"{k=}")  # start node
    if L == 2 ** k:
        g.append((k, k - 1, 0))
        g.append((k, k - 1, 2 ** (k - 1)))
    else:
        k -= 1
        i = k - 1
        while i >= 0:
            if (L - 1) & (1 << i):
                x = (L - 1) & ~((1 << (i + 1)) - 1)
                g.append((k, i, x))
            i -= 1
        g.append((k, 0, L - 1))
    return g, k


g, k = solve()
N = k + 1
M = len(g)
assert N <= 21
assert M <= 60
print(N, M)
for i in range(M):
    a, b, c = g[i]
    assert N - a < N - b
    print(N - a, N - b, c)
