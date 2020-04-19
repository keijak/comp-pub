import sys

sys.setrecursionlimit(10 ** 6)
N = int(input())
A = list(map(int, input().split()))
acts = [(x, i) for i, x in enumerate(A)]
acts.sort()
dp = [[0] * N for _ in range(N)]


def solve(l, r):
    if r < l:
        return 0
    if dp[l][r] >= 0:
        return dp[l][r]
    k = r - l
    act, init_pos = acts[k]
    gain_l = act * abs(l - init_pos)
    gain_r = act * abs(r - init_pos)
    dp[l][r] = max(gain_l + solve(l + 1, r), gain_r + solve(l, r - 1))
    return dp[l][r]


print(solve(0, N - 1))
