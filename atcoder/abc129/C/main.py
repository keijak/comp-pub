#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
A = list(map(int, readlines()))
assert len(A) == M

MOD = 10 ** 9 + 7


def solve():
    broken = [False] * (N + 1)
    for a in A:
        broken[a] = True
    p1, p2 = 1, 0
    for i in range(1, N + 1):
        if broken[i]:
            p1, p2 = 0, p1
        elif i == 1:
            p1, p2 = p1, p1
        else:
            p1, p2 = (p1 + p2) % MOD, p1
    return p1


if __name__ == "__main__":
    print(solve())
