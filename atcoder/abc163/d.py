N, K = map(int, input().split())
M = 10 ** 9 + 7
ans = 0
for k in range(K, N + 2):
    ans += (2 * N - k + 1) * k // 2 - k * (k - 1) // 2 + 1
    ans %= M
print(ans)
