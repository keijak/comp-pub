import heapq

N, M, S = map(int, input().split())
S -= 1
adj = [[] for _ in range(N)]
for i in range(M):
    u, v = [int(x) - 1 for x in input().split()]
    adj[u].append(v)
    adj[v].append(u)


def solve():
    visited = [False] * N
    minnodes = [S] * N
    hp = [(-S, S)]
    while hp:
        minnode, node = heapq.heappop(hp)
        minnode = -minnode
        if visited[node]:
            continue
        visited[node] = True
        # print(f"visiting {node}: blocker: {minnode}")
        minnodes[node] = minnode
        nextmin = min(minnode, node)
        for nb in adj[node]:
            if not visited[nb]:
                heapq.heappush(hp, (-nextmin, nb))
    return minnodes


minnodes = solve()
for i in range(N):
    if minnodes[i] >= i:
        print(i + 1)
