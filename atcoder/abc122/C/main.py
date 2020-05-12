#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, Q = map(int, readline().split())
S = readline().decode("utf-8").rstrip()
qs = []
for i in range(Q):
    l, r = map(int, readline().split())
    qs.append((l - 1, r - 1))


def solve():
    acc = [0] * (N + 1)
    for i in range(1, N + 1):
        acc[i] = acc[i - 1]
        if S[i - 1 : i + 1] == "AC":
            acc[i] += 1
    for l, r in qs:
        print(acc[r] - acc[l])


if __name__ == "__main__":
    solve()
