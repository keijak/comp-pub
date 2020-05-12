#!/usr/bin/env python3
import sys
import math

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

a, b, x = map(int, readline().split())


def solve():
    if 2 * x > a * a * b:
        return math.atan(2.0 * (b / a - x / (a * a * a)))
    else:
        return math.atan(0.5 * a * b * b / x)


if __name__ == "__main__":
    print(solve() / math.pi * 180.0)
