#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()


def solve():
    K = ni()
    S = ns()
    if len(S) > K:
        print(S[:K] + "...")
    else:
        print(S)

 
if __name__ == "__main__":
    solve()
