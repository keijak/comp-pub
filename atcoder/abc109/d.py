H, W = map(int, input().split())
coins = []
for i in range(H):
    a = list(map(int, input().split()))
    coins.append(a)

out = []


def move(x1, y1, x2, y2):
    if 0 <= x2 < H and 0 <= y2 < W:
        coins[x1][y1] -= 1
        coins[x2][y2] += 1
        out.append((x1 + 1, y1 + 1, x2 + 1, y2 + 1))


for i in range(0, H, 2):
    for j in range(W):
        if coins[i][j] % 2:
            if j < W - 1:
                move(i, j, i, j + 1)
            else:
                move(i, j, i + 1, j)
    for j in range(W - 1, -1, -1):
        if i < H - 1 and coins[i + 1][j] % 2:
            if j > 0:
                move(i + 1, j, i + 1, j - 1)
            else:
                move(i + 1, j, i + 2, j)

print(len(out))
for c in out:
    print(*c)
