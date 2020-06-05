import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, M, Q = inm()
adj = [[] for _ in range(N)]
for i in range(M):
    u, v = inm()
    u -= 1
    v -= 1
    assert u != v
    adj[u].append(v)
    adj[v].append(u)

color = inl()
assert len(color) == N


def solve():
    for _ in range(Q):
        qt, x, *ys = inm()
        x -= 1  # node
        if qt == 1:
            print(color[x])
            y = color[x]
            for nei in adj[x]:
                color[nei] = y
        else:
            assert qt == 2
            assert len(ys) == 1
            y = ys[0]  # color
            print(color[x])
            color[x] = y


solve()
