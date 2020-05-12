N = int(input())
t = [[0] * 10 for _ in range(10)]

for i in range(1, N+1):
    x = i % 10
    y = i
    while y // 10:
        y //= 10
    t[x][y] += 1

ans = 0
for i in range(1, 10):
    for j in range(1, 10):
        ans += t[i][j] * t[j][i]
print(ans)
