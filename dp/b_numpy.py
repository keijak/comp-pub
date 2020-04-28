import numpy as np

N, K = map(int, input().split())
H = np.array(list(map(int, input().split())), dtype=np.int64)
dp = np.ones(N, dtype=np.int64) * (2 ** 62)
dp[0] = 0
for i in range(1, N):
    cost = np.abs(H[i - 1] - H[i : i + K])
    np.minimum(dp[i : i + K], dp[i - 1] + cost, out=dp[i : i + K])
print(dp[-1])
