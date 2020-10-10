from collections import deque

N = int(input())
edges = []
nb = [[] for _ in range(N)]
maxdeg = 0
root = None
for i in range(N - 1):
    a, b = map(int, input().split())
    a -= 1
    b -= 1
    edges.append((min(a, b), max(a, b)))
    nb[a].append(b)
    nb[b].append(a)
    if len(nb[a]) > maxdeg:
        maxdeg = len(nb[a])
        root = a
    if len(nb[b]) > maxdeg:
        maxdeg = len(nb[b])
        root = b

colors = {}
for i, x in enumerate(nb[root]):
    colors[x, root] = colors[root, x] = i + 1
    q = deque([(x, root, i + 1)])
    while q:
        node, parent, pcolor = q.popleft()
        colors[node, parent] = colors[parent, node] = pcolor
        color = 0
        for c in nb[node]:
            if c != parent:
                color += 1
                if color == pcolor:
                    color += 1
                q.append((c, node, color))

print(maxdeg)
for a, b in edges:
    print(colors[a, b])
