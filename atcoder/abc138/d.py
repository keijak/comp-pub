from collections import defaultdict
N, Q = map(int, input().split())

neighbor = defaultdict(list)
counter = [0] * N

for i in range(N-1):
    a, b = map(int, input().split())
    neighbor[a-1].append(b-1)
    neighbor[b-1].append(a-1)

for i in range(Q):
    p, x = map(int, input().split())
    counter[p-1] += x

def propagate():
    stack = [(0, None)]
    while stack:
        (node, parent) = stack.pop()
        for c in neighbor[node]:
            if c != parent:
                counter[c] += counter[node]
                stack.append((c, node))

propagate()
print(*counter)
