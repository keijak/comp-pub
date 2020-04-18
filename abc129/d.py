H,W = map(int, input().split())
grid = []
for i in range(H):
    grid.append(input())
horiz = [[None]*W for _ in range(H)]
horiv = [[] for _ in range(H)]
for i in range(H):
    cur = 0
    for j in range(W):
        if grid[i][j] == '.':
            horiz[i][j] = len(horiv[i])
        else:
            horiv[i].append(j - cur)
            cur = j + 1
    horiv[i].append(W-cur)

maxv = 0
for c in range(W):
    cur = 0
    besth = 0
    for r in range(H):
        if grid[r][c] == '.':
            besth = max(besth, horiv[r][horiz[r][c]])
        else:
            maxv = max(maxv, r-cur + besth - 1)
            cur = r+1
            besth = 0
    maxv = max(maxv, H-cur + besth - 1)

print(maxv)