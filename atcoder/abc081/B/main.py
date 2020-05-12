#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
A = list(map(int, readline().split()))


def solve():
    minc = 1000
    for a in A:
        c = 0
        while (a & 1) == 0:
            a >>= 1
            c += 1
        minc = min(minc, c)
    return minc


if __name__ == "__main__":
    print(solve())
