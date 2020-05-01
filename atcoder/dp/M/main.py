#!/usr/bin/env python3
import copy
import sys

MOD = 1000000007  # type: int


def debug(s):
    print(s)


def solve(N: int, K: int, a: "List[int]"):
    dp = [[0] * (K + 1) for _ in range(N + 1)]
    dp[0][0] = 1
    for i in range(1, N + 1):
        ai = a[i - 1]
        dp[i][0] = 1
        for k in range(1, K + 1):
            c = dp[i - 1][k] + dp[i][k - 1]
            if k - ai - 1 >= 0:
                c -= dp[i - 1][k - ai - 1]
            dp[i][k] = c % MOD
            # debug(f"{i} kids share {k} candies: => {dp[i][k]}")
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
