import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    groups = set()
    if n % 2 == 0:
        s = n + 1
    else:
        s = n
    for i in range(1, n // 2 + 1):
        groups.add((i, s - i))

    edges = set()
    for i in range(1, n + 1):
        for j in range(i + 1, n + 1):
            if (i, j) not in groups:
                edges.add((i, j))
    return edges


edges = solve()
print(len(edges))
for x, y in sorted(edges):
    print(x, y)
