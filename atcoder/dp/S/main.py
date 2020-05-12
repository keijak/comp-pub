#!/usr/bin/env python3
import sys

MOD = 1000000007  # type: int


def solve(K: int, D: int):

    dp = [[0] * D for _ in range(3)]
    dp[0][0] = 1
    for i in range(1, 3):
        for j in range(D):
            for k in range(10):
                dp[i][(10 * j % D + k) % D] += dp[i - 1][j]
    return dp


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    K = int(next(tokens))  # type: int
    D = int(next(tokens))  # type: int
    print(solve(K, D))


if __name__ == "__main__":
    main()
