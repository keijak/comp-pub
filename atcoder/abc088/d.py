import collections

H,W = map(int,input().split())
maze = []
for i in range(H):
    maze.append(list(input()))
nodes = []
nodedict = {}
for i in range(H):
    for j in range(W):
        if maze[i][j] == '.':
            nid = len(nodes)
            nodedict[(i,j)] = nid
            nodes.append((i,j))

N = len(nodes)
INF = H*W
edges = [[False]*N for _ in range(N)]

def link(i1,j1,i2,j2):
    n1 = nodedict[i1,j1]
    n2 = nodedict[i2,j2]
    edges[n1][n2] = edges[n2][n1] = True

for i in range(H):
    for j in range(W):
        if maze[i][j] != '.': continue
        if i > 0 and maze[i-1][j]=='.': link(i,j,i-1,j)
        if i < H-1 and maze[i+1][j]=='.': link(i,j,i+1,j)
        if j > 0 and maze[i][j-1]=='.': link(i,j,i,j-1)
        if j < W-1 and maze[i][j+1]=='.': link(i,j,i,j+1)

dist = [INF] * len(nodes)

def solve():
    dist[0] = 0
    n0 = nodedict[(0, 0)]
    q = collections.deque([(0, n0)])
    determined = {n0}
    while q:
        d, nid = q.popleft()
        for i, linked in enumerate(edges[nid]):
            if linked and i not in determined:
                dist[i] = min(dist[i], d+1)
                determined.add(i)
                q.append((d+1, i))
    goal = nodedict[(H-1, W-1)]
    if dist[goal] == INF:
        return -1
    return len(nodes) - dist[goal] - 1

print(solve())