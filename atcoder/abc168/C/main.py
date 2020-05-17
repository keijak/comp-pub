#!/usr/bin/env python3
import sys
import math

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(float, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

A, B, H, M = nm()


def solve():
    tb = M / 60.0 * (2 * math.pi)
    ta = (H + (M / 60.0)) / (12.0) * (2.0 * math.pi)
    C2 = A ** 2.0 + B ** 2.0 - 2.0 * A * B * math.cos(abs(tb - ta))
    return math.sqrt(C2)


if __name__ == "__main__":
    print(solve())
