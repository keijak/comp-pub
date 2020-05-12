import itertools
from collections import Counter
H, W = map(int, input().split())
cost = []
for i in range(10):
    cost.append(list(map(int, input().split())))
wall = []
for i in range(H):
    wall.append(list(map(int, input().split())))

for k in range(10):
    for i in range(10):
        for j in range(10):
            cost[i][j] = min(cost[i][j], cost[i][k] + cost[k][j])

wc = Counter(itertools.chain(*wall))
del wc[-1]
del wc[1]

ans = 0
for k, v in wc.items():
    ans += cost[k][1] * v
print(ans)