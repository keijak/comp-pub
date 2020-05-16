#!/usr/bin/env python3
import sys
from fractions import gcd

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A = nl()
assert len(A) == N


def solve():
    al = [None] * N
    ar = [None] * N
    al[0] = A[0]
    ar[-1] = A[-1]
    for i in range(1, N):
        al[i] = gcd(al[i - 1], A[i])
        ar[N - 1 - i] = gcd(ar[N - i], A[N - 1 - i])
    gmax = max(al[-2], ar[1])
    gmi = 0 if gmax == ar[1] else N - 1
    for i in range(1, N - 1):
        g = gcd(al[i - 1], ar[i + 1])
        if gmax < g:
            gmax = g
            gmi = i
    return gmax


if __name__ == "__main__":
    print(solve())
