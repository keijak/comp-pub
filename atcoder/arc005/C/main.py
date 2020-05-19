import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

H, W = nm()
grid = []
spos = gpos = None
for r in range(H):
    line = ns()
    grid.append(line)
    for i, c in enumerate(line):
        if c == "s":
            assert spos is None
            spos = (r, i)
        elif c == "g":
            assert gpos is None
            gpos = (r, i)

# print(">>>", spos, gpos)


def solve():
    que = collections.deque()
    que.append((spos[0], spos[1], 0))
    vis = [[3] * W for _ in range(H)]
    while que:
        py, px, br = que.popleft()
        if vis[py][px] <= br:
            continue
        vis[py][px] = br
        if (py, px) == gpos and br <= 2:
            return True
        for dy, dx in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
            qy = py + dy
            qx = px + dx
            if 0 <= qy < H and 0 <= qx < W:
                if grid[qy][qx] == "#":
                    if br < 2:
                        newbr = br + 1
                    else:
                        continue
                else:
                    newbr = br
                if vis[qy][qx] > newbr:
                    que.append((qy, qx, newbr))
    return False


if __name__ == "__main__":
    print("YES" if solve() else "NO")
