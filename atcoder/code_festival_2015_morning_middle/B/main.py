#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.buffer.readline())
nm = lambda: map(int, sys.stdin.buffer.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
S = ns()
assert len(S) == N


def lcs(s1, s2):
    if not s1 or not s2:
        return 0
    dp = [[0] * len(s2) for _ in s1]
    dp[0][0] = int(s1[0] == s2[0])
    for i in range(1, len(s2)):
        dp[0][i] = max(dp[0][i - 1], int(s1[0] == s2[i]))
    for i in range(1, len(s1)):
        dp[i][0] = max(dp[i - 1][0], int(s1[i] == s2[0]))
        for j in range(1, len(s2)):
            t = max(dp[i - 1][j], dp[i][j - 1])
            dp[i][j] = max(t, dp[i - 1][j - 1] + int(s1[i] == s2[j]))
    return dp[-1][-1]


def solve():
    ans = N
    for i in range(N):
        x = lcs(S[:i], S[i:])
        ans = min(ans, N - 2 * x)
    return ans


if __name__ == "__main__":
    print(solve())
