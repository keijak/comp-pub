import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

M = 10 ** 9 + 7
H, W = map(int, readline().split())
grid = []
for i in range(H):
    grid.append(readline().decode("utf-8").rstrip())
    assert len(grid[-1]) == W


def solve():
    t = [0] * W
    t[0] = 1
    for c in range(1, W):
        if grid[0][c] == ".":
            t[c] = t[c - 1]
    for r in range(1, H):
        for c in range(W):
            if grid[r][c] == "#":
                t[c] = 0
            elif c > 0:
                t[c] += t[c - 1]
                t[c] %= M
        print(r, "\t", t)
    return t[-1]


if __name__ == "__main__":
    print(solve())
