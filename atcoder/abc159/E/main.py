#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

INF = 1 << 60
H, W, K = nm()
grid = []
for i in range(H):
    row = ns()
    assert len(row) == W
    grid.append(row)


def min_vsplits(ranges):
    vsplits = 0
    block_count = [0] * len(ranges)
    line_count = [0] * len(ranges)
    for c in range(W):
        for i in range(len(line_count)):
            line_count[i] = 0
        for i, (start, end) in enumerate(ranges):
            for r in range(start, end):
                if grid[r][c] == "1":
                    line_count[i] += 1
                    block_count[i] += 1
        if max(line_count) > K:
            return INF  # one column contains more than K.
        if max(block_count) > K:
            vsplits += 1
            for i in range(len(block_count)):
                block_count[i] = line_count[i]
    return vsplits


def solve():
    S = 1 << (H - 1)
    min_splits = (H - 1) * (W - 1)  # split all
    for splits in range(S):
        ranges = []
        start = 0
        stop = 1
        while splits:
            if splits & 1:
                ranges.append((start, stop))
                start = stop
            splits >>= 1
            stop += 1
        ranges.append((start, H))
        hsplits = len(ranges) - 1
        vsplits = min_vsplits(ranges)
        s = hsplits + vsplits
        if min_splits > s:
            min_splits = s
            # print(
            #     f"min_splits={min_splits} ({hsplits}+{vsplits}) ranges={ranges}",
            #     file=sys.stderr,
            # )
    return min_splits


print(solve())
