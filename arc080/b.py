H, W = map(int, input().split())
N = int(input())
A = list(map(int, input().split()))

color = 1
color_count = A[color-1]

grid = [[None] * W for _ in range(H)]
for i in range(H):
    if i % 2 == 0:
        rng = range(W)
    else:
        rng = range(W-1,-1,-1)
    for j in rng:
        if color_count == 0:
            color += 1
            color_count = A[color-1]
        color_count -= 1
        grid[i][j] = color

for i in range(H):
    print(*grid[i], sep=' ')

