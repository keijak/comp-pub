from collections import defaultdict

N, M, K = map(int, input().split())
friends = defaultdict(list)
blocked = defaultdict(list)

for i in range(M):
    a, b = map(int, input().split())
    friends[a - 1].append(b - 1)
    friends[b - 1].append(a - 1)
for j in range(K):
    c, d = map(int, input().split())
    blocked[c - 1].append(d - 1)
    blocked[d - 1].append(c - 1)

groups = {}
for i in range(N):
    if i in groups:
        continue
    group = []
    stack = [(i, None)]
    while stack:
        j, parent_id = stack.pop()
        if j in groups:
            continue
        group.append(j)
        groups[j] = group
        for k in friends[j]:
            if k != parent_id:
                stack.append((k, j))

ans = []
for i in range(N):
    g = groups[i]
    res = len(g) - len(friends[i]) - 1
    for b in blocked[i]:
        if groups[b] is g:
            res -= 1
    ans.append(res)
print(*ans)
