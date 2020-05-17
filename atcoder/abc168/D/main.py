#!/usr/bin/env python3
import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, M = nm()
adj = [[] for _ in range(N)]
for i in range(M):
    a, b = nm()
    a -= 1
    b -= 1
    adj[a].append(b)
    adj[b].append(a)


def solve():
    INF = 1 << 62
    q = collections.deque([(0, 0, -1)])
    dist = [INF] * N
    prev = [None] * N
    while q:
        l, node, pnode = q.popleft()
        if dist[node] < l:
            continue
        dist[node] = l
        prev[node] = pnode
        for nxt in adj[node]:
            if nxt != pnode and dist[nxt] > l + 1:
                q.append((l + 1, nxt, node))
    # if any(x is None for x in prev):
    #     print("No")
    #     return
    print("Yes")
    for i in range(1, N):
        print(prev[i] + 1)


if __name__ == "__main__":
    solve()
