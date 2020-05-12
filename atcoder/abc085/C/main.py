#!/usr/bin/env python3
import sys
import collections

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, Y = map(int, readline().split())
assert Y % 1000 == 0
Y //= 1000


def solve():
    for x1 in range(N + 1):
        for x2 in range(N - x1 + 1):
            x3 = N - x1 - x2
            if 10 * x1 + 5 * x2 + 1 * x3 == Y:
                print(x1, x2, x3)
                return
    print(-1, -1, -1)


if __name__ == "__main__":
    solve()
