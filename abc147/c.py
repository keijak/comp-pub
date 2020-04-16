N = int(input())
edges = [[None] * N for _ in range(N)]
for i in range(N):
    num_prop = int(input())
    for j in range(num_prop):
        x, y = map(int, input().split())
        edges[i][x-1] = y


def check(hs, us):
    for h in hs:
        for h2 in hs:
            if edges[h][h2] == 0:
                return False
        for u in us:
            if edges[h][u] == 1:
                return False
    return True


def search():
    maxv = 0
    for i in range(1 << N):
        hs = []
        us = []
        for j in range(N):
            if (i >> j) & 1 == 1:
                hs.append(j)
            else:
                us.append(j)
        if check(hs, us):
            if maxv < len(hs):
                maxv = len(hs)
    return maxv

print(search())
j