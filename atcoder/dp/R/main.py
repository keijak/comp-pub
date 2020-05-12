#!/usr/bin/env python3
import sys

MOD = 1000000007  # type: int


def solve(N: int, K: int, adj):
    dp = [[0] * N for _ in range(2 * N)]
    for i in range(N):
        dp[1][i] = len(adj[i])
    for k in range(2, K + 1):
        for i in range(N):
            for j in adj[i]:
                dp[k][i] += dp[k - 1][j]
                dp[k][i] %= MOD
        print(f"> dp[{k}][0]={dp[k][0]}", file=sys.stderr, flush=k % 10 == 0)
    ans = 0
    for i in range(N):
        ans += dp[K][i]
        ans %= MOD
    return ans


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    K = int(next(tokens))  # type: int
    adj = [[] for _ in range(N)]
    for i in range(N):
        for j in range(N):
            a = int(next(tokens))
            if a:
                adj[i].append(j)
    print(solve(N, K, adj))


if __name__ == "__main__":
    main()
