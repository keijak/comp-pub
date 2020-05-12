import heapq
n, m = map(int,input().split())
a = list(map(int,input().split()))
assert len(a) == n

hp = []
for x in a:
    item = (-x, x, 0)
    heapq.heappush(hp, item)

for i in range(m):
    (neg_cost, full_price, n) = heapq.heappop(hp)
    new_cost = full_price // (2 ** (n+1))
    item = (-new_cost, full_price, n+1)
    heapq.heappush(hp, item)
    if neg_cost == 0:
        break

print(sum(-c for (c, _, _) in hp))