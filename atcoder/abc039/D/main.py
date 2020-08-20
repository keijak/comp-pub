import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    H, W = inl()
    grid = [list(ins()) for _ in range(H)]
    white = [[False] * W for _ in range(H)]

    deltas = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
    for r in range(H):
        for c in range(W):
            if grid[r][c] == ".":
                continue
            for dr, dc in deltas:
                r2 = r + dr
                c2 = c + dc
                if 0 <= r2 < H and 0 <= c2 < W and grid[r2][c2] == ".":
                    white[r][c] = True
    for r in range(H):
        for c in range(W):
            if not white[r][c]:
                continue
            ok = False
            for dr, dc in deltas:
                r2 = r + dr
                c2 = c + dc
                if (
                    0 <= r2 < H
                    and 0 <= c2 < W
                    and grid[r2][c2] == "#"
                    and not white[r2][c2]
                ):
                    ok = True
                    break
            if not ok:
                return False

    print("possible")
    for r in range(H):
        for c in range(W):
            if white[r][c]:
                grid[r][c] = "."
    for r in range(H):
        print("".join(grid[r]))
    return True


if not solve():
    print("impossible")
