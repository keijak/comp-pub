#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines


def solve(N: int, a: "List[int]"):
    N1 = N + 1
    dpmin = [-1] * N1 ** 2
    dpmax = [-1] * N1 ** 2
    for i in range(N1):
        dpmin[i * N1 + i] = dpmax[i * N1 + i] = 0
    for i in range(N):
        dpmin[i * N1 + i + 1] = -a[i]
        dpmax[i * N1 + i + 1] = a[i]

    def minimize(a, begin, end):
        x = dpmin[begin * N1 + end]
        if x >= 0:
            return x
        pt1 = -a[begin] + maximize(a, begin + 1, end)
        pt2 = -a[end - 1] + maximize(a, begin, end - 1)
        r = min(pt1, pt2)
        dpmin[begin * N1 + end] = r
        return r

    def maximize(a, begin, end):
        x = dpmax[begin * N1 + end]
        if x >= 0:
            return x
        pt1 = a[begin] + minimize(a, begin + 1, end)
        pt2 = a[end - 1] + minimize(a, begin, end - 1)
        r = max(pt1, pt2)
        dpmax[begin * N1 + end] = r
        return r

    return maximize(a, 0, N)


def main():
    N = int(readline())  # type: int
    a = [int(x) for x in readline().split()]  # type: "List[int]"
    print(solve(N, a))


if __name__ == "__main__":
    main()
