#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def solve():
    n = ini()
    A = [None] * n
    B = [None] * n
    middle = 0
    for i in range(n):
        a, b = inl()
        A[i] = a
        B[i] = b
        assert a < b
        middle += b - a
    xa = sorted(A)[n // 2]
    xb = sorted(B)[n // 2]
    sa = sum([abs(a - xa) for a in A])
    sb = sum([abs(b - xb) for b in B])
    return sa + middle + sb


print(solve())
