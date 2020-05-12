#!/usr/bin/env python3
import sys
import copy

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

W = int(readline())
N, K = map(int, readline().split())
items = []
valuesum = 0
for i in range(N):
    width, value = map(int, readline().split())
    items.append((width, value))
    valuesum += value


def solve():
    dp = [[W + 1] * (valuesum + 1) for _ in range(K + 1)]
    w0, v0 = items[0]
    dp[0][0] = 0
    dp[1][v0] = w0
    for i in range(1, N):
        wi, vi = items[i]
        for k in range(min(i + 1, K), 0, -1):
            for vp in range(valuesum - vi + 1):
                wp = dp[k - 1][vp]
                if dp[k][vp + vi] > wp + wi:
                    dp[k][vp + vi] = wp + wi

    for v in range(valuesum, -1, -1):
        for k in range(K + 1):
            if dp[k][v] <= W:
                return v
    return 0


if __name__ == "__main__":
    print(solve())
