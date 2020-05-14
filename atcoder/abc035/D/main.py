#!/usr/bin/env python3
import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.buffer.readline())
nm = lambda: map(int, sys.stdin.buffer.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, M, T = nm()
A = nl()
assert len(A) == N

adjf = [[] for _ in range(N)]
adjr = [[] for _ in range(N)]


for i in range(M):
    a, b, c = nm()
    a -= 1
    b -= 1
    adjf[a].append((b, c))
    adjr[b].append((a, c))


def dijkstra(adj):
    INF = 1 << 62
    q = [(0, 0)]
    dist = [INF] * N
    while q:
        d, node = heapq.heappop(q)
        if d > dist[node]:
            continue
        dist[node] = d
        for nxt, step in adj[node]:
            if dist[nxt] > d + step:
                heapq.heappush(q, (d + step, nxt))
    return dist


def solve():
    df = dijkstra(adjf)
    dr = dijkstra(adjr)
    ans = T * A[0]
    ansi = 0
    for i in range(1, N):
        t = T - df[i] - dr[i]
        if t > 0 and ans < t * A[i]:
            ans = t * A[i]
            ansi = i
    return ans


if __name__ == "__main__":
    print(solve())
