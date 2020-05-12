# Max Flow (Ford-Fulkerson)
N, G, E = map(int, input().split())
SOURCE, SINK = 0, N
N += 1
targets = [int(x) for x in input().split()]
nb = [[False] * N for _ in range(N)]
for i in range(E):
    a, b = [int(x) for x in input().split()]
    nb[a][b] = True
    nb[b][a] = True
for t in targets:
    nb[t][SINK] = True


def dfs():
    stack = [(SOURCE, -1)]
    backtrack = [None] * N
    while stack:
        node, parent_node = stack.pop()
        if backtrack[node] is not None:
            continue
        backtrack[node] = parent_node
        if node == SINK:
            while True:
                p = backtrack[node]
                if p < 0:
                    return True
                nb[p][node] = False
                nb[node][p] = True
                node = p
        for child_node in range(N):
            if nb[node][child_node] and backtrack[child_node] is None:
                stack.append((child_node, node))
    return False


def max_flow():
    c = 0
    while dfs():
        c += 1
    return c


print(max_flow())
