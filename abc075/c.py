N,M = map(int, input().split())
linked = [[False] * N for _ in range(N)]
edges = []
for i in range(M):
    a, b = map(int, input().split())
    edges.append((a-1, b-1))
    linked[a-1][b-1] = linked[b-1][a-1] = True

def reach(s, g, visited):
    visited.add(s)
    for i in range(N):
        if not linked[s][i]:
            continue
        if i == g:
            return True
        if i not in visited and reach(i, g, visited):
            return True
    return False

cnt = 0
for i in range(M):
    a, b = edges[i]
    linked[a][b] = linked[b][a] = False
    if not reach(a, b, set()):
        cnt += 1
    linked[a][b] = linked[b][a] = True
print(cnt)
