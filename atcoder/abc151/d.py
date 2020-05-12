H, W = map(int, input().split())
maze = []
for _ in range(H):
    maze.append(input())
nodes = []
nodedict = {}
for r in range(H):
    for c in range(W):
        if maze[r][c] == '.':
            node_id = len(nodes)
            nodedict[(r,c)] = node_id
            nodes.append((r,c))
N = len(nodes)
INF = H*W+100
dist = [[INF] * N for _ in range(N)]
for i in range(N):
    dist[i][i] = 0

def connect(r1, c1, r2, c2):
    n1 = nodedict[(r1, c1)]
    n2 = nodedict[(r2, c2)]
    dist[n1][n2] = 1
    dist[n2][n1] = 1

def solve():
    for i in range(N):
        r, c = nodes[i]
        if r > 0 and maze[r-1][c] == '.':
            connect(r, c, r-1, c)
        if r < H-1 and maze[r+1][c] == '.':
            connect(r, c, r+1, c)
        if c > 0 and maze[r][c-1] == '.':
            connect(r, c, r, c-1)
        if c < W-1 and maze[r][c+1] == '.':
            connect(r, c, r, c+1)

    # Warshall Floyd
    for k in range(N):
        for i in range(N):
            for j in range(N):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    maxd = 0
    di, dj = None, None
    for i in range(N):
        for j in range(N):
            d = dist[i][j]
            if d < INF and maxd < d:
                maxd = d
                di = nodes[i]
                dj = nodes[j]
    return maxd  #, di, dj


print(solve())
