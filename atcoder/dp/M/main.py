#!/usr/bin/env python3
import copy
import sys
import numpy as np

M = 1000000007


def solve(N: int, K: int, a):
    dp = np.zeros(K + 1, dtype=np.int64)
    dp[0] = 1
    for x in a:
        cm = np.cumsum(dp)
        dp = cm.copy()
        dp[x + 1 :] -= cm[: -x - 1]
        dp %= M
    return dp[K]


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    K = int(next(tokens))  # type: int
    a = np.array([int(next(tokens)) for _ in range(N)], dtype=np.int64)
    print(solve(N, K, a))


if __name__ == "__main__":
    main()
