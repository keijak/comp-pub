N, K = map(int, input().split())
H = list(map(int, input().split()))
dp = [1 << 62] * N
dp[0] = 0
for i in range(1, N):
    for j in range(i, min(i + K, N)):
        cost = abs(H[i - 1] - H[j])
        dp[j] = min(dp[j], dp[i - 1] + cost)
print(dp[-1])
