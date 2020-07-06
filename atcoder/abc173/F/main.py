import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()


def solve():
    edge_count = 0
    for i in range(N - 1):
        u, v = sorted(inl())
        edge_count += u * (N - v + 1)

    node_count = 0
    for i in range(N):
        node_count += (i + 1) * (N - i)
    return node_count - edge_count


print(solve())
