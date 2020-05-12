import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, W = map(int, readline().split())
things = []
vsum = 0
for i in range(N):
    weight, value = map(int, readline().split())
    things.append((weight, value))
    vsum += value


INF = 1 << 62


def solve():
    dp = INF * np.ones(vsum + 1, dtype=int)
    w0, v0 = things[0]
    dp[0] = 0
    dp[v0] = w0
    for i in range(1, N):
        w, v = things[i]
        np.minimum(dp[v:], dp[:-v] + w, out=dp[v:])
    return np.where(dp <= W)[0][-1]


if __name__ == "__main__":
    print(solve())
