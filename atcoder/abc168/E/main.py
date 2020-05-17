#!/usr/bin/env python3
import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A, B = [], []
MOD = 1000000007

for i in range(N):
    a, b = nm()
    A.append(a)
    B.append(b)


def solve():
    dis = [0] * N
    ctr = collections.Counter()
    for i in range(N):
        a, b = A[i], B[i]
        cnt = ctr[b, -a] + ctr[-b, a]
        dis[i] = cnt
        ctr[a, b] += 1

    dp = [[0] * 2 for _ in range(N)]
    dp[0][0] = 1
    dp[0][1] = 1
    for i in range(1, N):
        dp[i][0] = (dp[i - 1][0] + dp[i - 1][1]) % MOD
        dp[i][1] = pow(2, i - dis[i], MOD)
    return (dp[N - 1][0] + dp[N - 1][1] - 1) % MOD


if __name__ == "__main__":
    print(solve())
