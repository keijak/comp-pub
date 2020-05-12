#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, readline().split())
KMAX = (N - 1) * (N - 2) // 2


def solve():
    if K > KMAX:
        return None
    edges = []
    for i in range(1, N):
        edges.append((1, i + 1))
    if K == KMAX:
        return edges
    cnt = KMAX
    for i in range(1, N):
        for j in range(i + 1, N):
            edges.append((i + 1, j + 1))
            cnt -= 1
            if cnt == K:
                return edges
    raise ValueError()


if __name__ == "__main__":
    r = solve()
    if r is None:
        print(-1)
    else:
        print(len(r))
        for u, v in r:
            print(u, v)
