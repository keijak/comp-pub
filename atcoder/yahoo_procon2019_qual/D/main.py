L = int(input())
A = [int(input()) for _ in range(L)]
dp = [[1 << 60] * 5 for _ in range(L + 1)]
dp[0] = [0] * 5
for i in range(1, L + 1):
    a = A[i - 1]
    dp[i][0] = dp[i - 1][0] + a
    tm = min(dp[i - 1][0], dp[i - 1][1])
    dp[i][1] = tm + (2 if a == 0 else a % 2)
    tm = min(tm, dp[i - 1][2])
    dp[i][2] = tm + (1 - a % 2)
    tm = min(tm, dp[i - 1][3])
    dp[i][3] = tm + (2 if a == 0 else a % 2)
    tm = min(tm, dp[i - 1][4])
    dp[i][4] = tm + a
print(min(dp[L]))
