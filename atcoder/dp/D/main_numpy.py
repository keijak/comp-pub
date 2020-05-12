import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, W = map(int, readline().split())
things = []
for i in range(N):
    weight, value = map(int, readline().split())
    things.append((weight, value))


def solve():
    dp = np.zeros(W + 1, dtype=int)
    w0, v0 = things[0]
    dp[w0:] = v0
    for i in range(1, N):
        weight, value = things[i]
        np.maximum(dp[weight:], dp[:-weight] + value, out=dp[weight:])
    return dp[W]


if __name__ == "__main__":
    print(solve())
