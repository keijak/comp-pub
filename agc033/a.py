import sys
import collections

input = sys.stdin.readline

H, W = map(int, input().split())
q = collections.deque()
grid = []
for i in range(H):
    grid.append(list(input().strip()))
    for j in range(W):
        if grid[i][j] == "#":
            q.append((i, j, 0))
maxc = 0
while q:
    i, j, c = q.popleft()
    if maxc < c:
        maxc = c
    if i > 0 and grid[i - 1][j] == ".":
        q.append((i - 1, j, c + 1))
        grid[i - 1][j] = "#"
    if i < H - 1 and grid[i + 1][j] == ".":
        q.append((i + 1, j, c + 1))
        grid[i + 1][j] = "#"
    if j > 0 and grid[i][j - 1] == ".":
        q.append((i, j - 1, c + 1))
        grid[i][j - 1] = "#"
    if j < W - 1 and grid[i][j + 1] == ".":
        q.append((i, j + 1, c + 1))
        grid[i][j + 1] = "#"

print(maxc)
