import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, M = inm()
cs = [None] * N
for i in range(N):
    cs[i] = ini()
adj = [[] for _ in range(N)]
for i in range(M):
    a, b, r = inm()
    a -= 1
    b -= 1
    adj[a].append((b, r))
    adj[b].append((a, r))

INF = 1 << 50


def solve():
    hp = []
    for i in range(N):
        heapq.heappush(hp, (cs[i], i))

    costs = [INF] * N
    total_cost = 0
    cnt = 0
    while hp:
        cost, node = heapq.heappop(hp)
        if costs[node] != INF:
            continue
        costs[node] = cost
        total_cost += cost
        cnt += 1
        if cnt == N:
            break
        for x, r in adj[node]:
            if costs[x] == INF:
                heapq.heappush(hp, (r, x))
    assert cnt == N
    return total_cost


print(solve())
