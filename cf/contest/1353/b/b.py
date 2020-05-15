#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines


def f(k, n, a, b):
    s = 0
    for i in range(k):
        if a[i] < b[i]:
            s += b[i]
        else:
            s += a[i]
    for i in range(k, n):
        s += a[i]
    return s


def solve():
    T = int(readline())
    for i in range(T):
        n, k = map(int, readline().split())
        a = list(map(int, readline().split()))
        b = list(map(int, readline().split()))
        assert len(a) == n
        assert len(b) == n
        a.sort()
        b.sort(reverse=True)
        print(f(k, n, a, b))


if __name__ == "__main__":
    solve()
