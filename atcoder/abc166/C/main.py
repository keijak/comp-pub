#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
H = list(map(int, readline().split()))
good = [True] * N
for i in range(M):
    a, b = [int(x) - 1 for x in readline().split()]
    if H[a] <= H[b]:
        good[a] = False
    if H[a] >= H[b]:
        good[b] = False


def solve():
    # for i in range(N):
    #     if good[i]:
    #         print(i, " is good")
    return sum(good)


if __name__ == "__main__":
    print(solve())
