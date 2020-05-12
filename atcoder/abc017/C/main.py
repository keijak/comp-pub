#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
lefts = [0] * (M + 2)
rights = [0] * (M + 2)
for i in range(N):
    l, r, s = map(int, readline().split())
    lefts[r + 1] += s
    rights[l - 1] += s


def solve():
    x = 0
    lacc = [0] * (M + 2)
    racc = [0] * (M + 2)
    for i in range(0, M + 2):
        x += lefts[i]
        lacc[i] = x
    x = 0
    for i in range(M + 1, -1, -1):
        x += rights[i]
        racc[i] = x
    best = 0
    for i in range(1, M + 1):
        score = lacc[i] + racc[i]
        if best < score:
            best = score
    return best


if __name__ == "__main__":
    print(solve())
