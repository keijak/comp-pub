#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines
A, B, C, K = map(int, readline().split())


def solve():
    if K <= A:
        return K
    elif K <= A + B:
        return A
    else:
        r = K - A - B
        return A - r


if __name__ == "__main__":
    print(solve())
