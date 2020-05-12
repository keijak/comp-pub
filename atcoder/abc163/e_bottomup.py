N = int(input())
A = list(map(int, input().split()))
acts = [(x, i) for i, x in enumerate(A)]
acts.sort()
dp = [[-1] * N for _ in range(N)]
for i in range(N):
    a, j = acts[0]
    dp[i][i] = a * abs(j - i)
for width in range(1, N):
    a, j = acts[width]
    for l in range(N - width):
        r = l + width
        dp[l][r] = max(a * abs(j - l) + dp[l + 1][r], a * abs(j - r) + dp[l][r - 1])

print(dp[0][N - 1])
