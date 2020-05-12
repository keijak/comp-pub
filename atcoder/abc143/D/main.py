#!/usr/bin/env python3
import sys
import bisect

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
L = list(map(int, readline().split()))
L.sort()


def solve():
    ans = 0
    for i in range(N):
        b = L[i]
        for j in range(i):
            a = L[j]
            k = bisect.bisect_left(L, a + b, i + 1) - 1
            ans += k - i
    return ans


if __name__ == "__main__":
    print(solve())
