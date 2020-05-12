N, x = map(int, input().split())
candies = list(map(int, input().split()))

eat = 0
for i in range(1, N):
    cur = candies[i] + candies[i - 1]
    if cur <= x:
        continue
    eat_i = cur - x
    c = min(eat_i, candies[i])
    candies[i] -= c
    candies[i - 1] -= eat_i - c
    eat += eat_i
print(eat)
