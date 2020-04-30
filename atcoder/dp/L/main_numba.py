#!/usr/bin/env python3
import sys

import numpy as np
import numba

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines


@numba.njit("i8(i8, i8[:])", cache=False)
def solve(N: int, a: "Sequence[int]"):
    N1 = N + 1
    dpmin = [-1] * N1 ** 2
    dpmax = [-1] * N1 ** 2
    for i in range(N1):
        dpmin[0 * N1 + i] = dpmax[0 * N1 + i] = 0
    for i in range(N):
        dpmin[1 * N1 + i] = -a[i]
        dpmax[1 * N1 + i] = a[i]
    for k in range(2, N1):
        for i in range(N - k + 1):
            dpmin[k * N1 + i] = min(
                -a[i] + dpmax[(k - 1) * N1 + i + 1],
                -a[i + k - 1] + dpmax[(k - 1) * N1 + i],
            )
            dpmax[k * N1 + i] = max(
                a[i] + dpmin[(k - 1) * N1 + i + 1],
                a[i + k - 1] + dpmin[(k - 1) * N1 + i],
            )
    return dpmax[N * N1]


def main():
    N = int(readline())  # type: int
    a = [int(x) for x in readline().split()]  # type: "List[int]"
    na = np.array(a, dtype=np.int64)
    print(solve(N, na))


if __name__ == "__main__":
    main()
