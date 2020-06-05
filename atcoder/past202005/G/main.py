import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, X, Y = inm()
INF = 1 << 30
L = 600
OFS = L // 2

obst = [[False] * L for _ in range(L)]
for i in range(N):
    x, y = inm()
    obst[OFS + x][OFS + y] = True
steps = [(+1, +1), (0, +1), (-1, +1), (+1, 0), (-1, 0), (0, -1)]


def solve():
    dist = [[INF] * L for _ in range(L)]
    q = collections.deque()
    q.append((OFS, OFS, 0))
    while q:
        xo, yo, d = q.popleft()
        if dist[xo][yo] <= d:
            continue
        dist[xo][yo] = d
        if xo == OFS + X and yo == OFS + Y:
            break
        # dbg(f"dist[{xo=},{yo=}]={d=}")
        for dx, dy in steps:
            xn, yn = xo + dx, yo + dy
            if 0 <= xn < L and 0 <= yn < L:
                if obst[xn][yn]:
                    continue
                if dist[xn][yn] <= d + 1:
                    continue
                q.append((xn, yn, d + 1))
    ans = dist[OFS + X][OFS + Y]
    if ans == INF:
        return -1
    return ans


print(solve())
