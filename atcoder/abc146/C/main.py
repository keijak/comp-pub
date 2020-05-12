#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

A, B, X = map(int, readline().split())


def solve():
    best = 0
    for d in range(1, 18):
        rem = X - B * d
        if rem <= 0:
            continue
        nmax = rem // A
        if nmax < 10 ** (d - 1):
            continue
        if nmax > 10 ** 9:
            nmax = 10 ** 9
        score = min(nmax, 10 ** d - 1)
        best = max(best, score)
    return best


if __name__ == "__main__":
    print(solve())
