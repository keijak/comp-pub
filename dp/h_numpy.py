import sys
import numpy as np
import numba

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

H, W = map(int, readline().split())
grid = np.zeros((H, W), dtype=np.int64)
for i in range(H):
    s = readline().decode("utf-8").rstrip()
    grid[i, :] = [c == "." for c in s]


@numba.njit(numba.i8(numba.i4, numba.i4, numba.typeof(grid)), cache=False)
def solve(H, W, grid):
    M = 10 ** 9 + 7
    t = grid[0, :].copy()
    for c in range(1, W):
        t[c] = min(t[c], t[c - 1])
    for r in range(1, H):
        mask = grid[r, :]
        t[0] *= mask[0]
        for c in range(1, W):
            t[c] += t[c - 1]
            t[c] *= mask[c]
            t[c] %= M
    return t[-1]


if __name__ == "__main__":
    print(solve(H, W, grid))
