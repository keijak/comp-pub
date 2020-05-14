#!/usr/bin/env python3
import sys
import collections
import heapq

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.buffer.readline())
nm = lambda: map(int, sys.stdin.buffer.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, M = nm()
adj = [[] for _ in range(N)]
for i in range(M):
    u, v, l = nm()
    u -= 1
    v -= 1
    adj[u].append((v, l))
    adj[v].append((u, l))


def solve():
    q = []
    INF = 1 << 62
    minloop = INF
    for nxt, l in adj[0]:
        heapq.heappush(q, (l, nxt, -1, nxt))
    pathlens = [None for _ in range(N)]
    pathlens[0] = (-1, 0)
    visited = [False] * N
    visited[0] = True
    while q:
        pathlen, node, pnode, color = heapq.heappop(q)
        if visited[node]:
            if node != 0:
                pcolor, pcost = pathlens[node]
                if pcolor != color:
                    minloop = min(minloop, pcost + pathlen)
            continue
        visited[node] = True
        pathlens[node] = (color, pathlen)
        for nxt, cost in adj[node]:
            if nxt != pnode:
                heapq.heappush(q, (cost + pathlen, nxt, node, color))
    if minloop == INF:
        return -1
    else:
        return minloop


if __name__ == "__main__":
    print(solve())
