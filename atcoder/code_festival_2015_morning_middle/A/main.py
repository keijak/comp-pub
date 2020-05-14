#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
ns = lambda: sys.stdin.readline().rstrip().decode("utf-8")
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())

N, K, M, R = nm()
S = []
for i in range(N - 1):
    S.append(ni())
S.sort(reverse=True)


def solve():
    topk0 = sum(S[:K])
    if topk0 >= R * K:
        return 0
    topk1 = sum(S[: K - 1])
    x = R * K - topk1
    if x > M:
        return -1
    return max(x, 0)


if __name__ == "__main__":
    print(solve())
