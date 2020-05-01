import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, readline().split())
A = np.array(list(map(int, input().split())))
A.sort()
amin, amax = A[0], A[-1]


def solve():
    dp = np.zeros(K + 1, dtype=bool)
    for i in range(amin, K + 1):
        dp[i] = np.any(~dp.take(i - A))
    return dp[K]


if __name__ == "__main__":
    print("First" if solve() else "Second")
