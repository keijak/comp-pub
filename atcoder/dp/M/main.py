#!/usr/bin/env python3
import copy
import sys

MOD = 1000000007  # type: int


def solve(N: int, K: int, a: "List[int]"):
    dp = [[0] * (K + 1) for _ in range(N + 1)]
    dp[0][0] = 1
    for k in range(min(a[0], K) + 1):
        dp[1][k] = 1
    for i in range(1, N + 1):
        acc = dp[i + 1][0] = 1
        for k in range(1, min(ai, K) + 1):
            dp[i + 1][k] = (dp[i + 1][k - 1] + dp[i][k]) % MOD
    return dp[N][K]


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    K = int(next(tokens))  # type: int
    a = [int(next(tokens)) for _ in range(N)]  # type: "List[int]"
    print(solve(N, K, a))


if __name__ == "__main__":
    main()
