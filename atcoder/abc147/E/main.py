#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

INF = 1 << 62
D = 81
H, W = nm()
A, B = [], []
for i in range(H):
    A.append(nl())
for i in range(H):
    B.append(nl())


def solve():
    dp = [[0 for _ in range(W)] for _ in range(H)]
    u0 = abs(A[0][0] - B[0][0])
    assert 0 <= u0 < D
    dp[0][0] = 1 << up

    return A


if __name__ == "__main__":
    print(solve())
