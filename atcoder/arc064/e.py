import math
import heapq

x1, y1, x2, y2 = map(int, input().split())
N = int(input())
circles = [(x1, y1, 0), (x2, y2, 0)]
for i in range(N):
    x, y, r = map(int, input().split())
    circles.append((x, y, r))
N += 2

dists = [[None] * N for _ in range(N)]
for i in range(N):
    xi, yi, ri = circles[i]
    for j in range(N):
        xj, yj, rj = circles[j]
        d = math.sqrt(abs(xi - xj) ** 2.0 + abs(yi - yj) ** 2.0) - ri - rj
        dists[i][j] = dists[j][i] = max(d, 0)

mincost = [None] * N
hp = [(0.0, 0)]
while hp:
    mc, node = heapq.heappop(hp)
    if mincost[node] is not None:
        continue
    mincost[node] = mc
    for i in range(N):
        if mincost[i] is None:
            heapq.heappush(hp, (mc + dists[node][i], i))
print(mincost[1])
