#!/usr/bin/env python3
import sys
import heapq
import collections

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

H, W, T = map(int, readline().split())
grid = []
start = goal = None
for r in range(H):
    s = readline().decode("utf-8").rstrip()
    assert len(s) == W
    grid.append(s)
    for c in range(W):
        if s[c] == "S":
            assert start is None
            start = (r, c)
        elif s[c] == "G":
            assert goal is None
            goal = (r, c)


def check(x):
    q = []
    heapq.heappush(q, (0, start))
    mind = collections.defaultdict(lambda: T + 1)
    while q:
        d, pos = heapq.heappop(q)
        if d > T:
            return False
        if pos == goal:
            return True
        if mind[pos] <= d:
            continue
        mind[pos] = d
        r, c = pos
        for (rdelta, cdelta) in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
            rn = r + rdelta
            cn = c + cdelta
            if (0 <= rn < H) and (0 <= cn < W):
                nxt = grid[rn][cn]
                if nxt == "#":
                    dn = d + x
                else:
                    dn = d + 1
                if dn < mind[(rn, cn)]:
                    heapq.heappush(q, (dn, (rn, cn)))
    return False


def solve():
    ok, ng = 1, T + 1
    while ng - ok > 1:
        m = (ng + ok) // 2
        if check(m):
            ok = m
        else:
            ng = m
    return ok


if __name__ == "__main__":
    print(solve())
