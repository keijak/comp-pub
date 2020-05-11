#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, readline().split())
A = [int(x) - 1 for x in readline().split()]
assert len(A) == N


def solve():
    cur = 0
    seen = {0: 0}
    tele = [0]
    loop = []
    loopenter = -1
    while True:
        nxt = A[cur]
        if nxt in seen:
            loopenter = seen[nxt]
            loop = tele[loopenter:]
            break
        seen[nxt] = len(tele)
        tele.append(nxt)
        cur = nxt

    if K < loopenter:
        return tele[K]
    k = (K - loopenter) % len(loop)
    return loop[k]


if __name__ == "__main__":
    print(solve() + 1)
