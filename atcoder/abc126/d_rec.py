"""DFS by recursive function.

TLE on Pypy3. Fast enough on CPython 3.4.3.
"""

import sys

sys.setrecursionlimit(10 ** 9)
input = sys.stdin.readline

N = int(input())
en = [[] for _ in range(N)]
on = [[] for _ in range(N)]
for i in range(N - 1):
    u, v, w = map(int, input().split())
    if w % 2:
        on[u - 1].append(v - 1)
        on[v - 1].append(u - 1)
    else:
        en[u - 1].append(v - 1)
        en[v - 1].append(u - 1)

color = [None] * N


def dfs(node, parent, col):
    if color[node] is not None:
        return
    color[node] = col
    for k in en[node]:
        if k != parent:
            dfs(k, node, col)
    for k in on[node]:
        if k != parent:
            dfs(k, node, (col + 1) % 2)


dfs(0, -1, 0)
print(*color, sep="\n")
