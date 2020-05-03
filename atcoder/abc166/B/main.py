#!/usr/bin/env python3

import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, readline().split())
cands = [0] * N
for i in range(K):
    _ = readline()
    a = list(map(int, readline().split()))
    for x in a:
        cands[x - 1] += 1


def solve():
    # print(cands)
    return sum(1 for c in cands if c == 0)


if __name__ == "__main__":
    print(solve())
