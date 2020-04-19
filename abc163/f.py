import collections
import sys

sys.setrecursionlimit(10 ** 6)

N = int(input())
colors = [int(x) - 1 for x in input().split()]
neighbors = collections.defaultdict(list)
for i in range(N - 1):
    a, b = map(int, input().split())
    neighbors[a - 1].append(b - 1)
    neighbors[b - 1].append(a - 1)

subtree_sizes = [0] * N


def calc_sizes(node, parent):
    sz = 1
    for i in neighbors[node]:
        if i != parent:
            sz += calc_sizes(i, node)
    subtree_sizes[node] = sz
    return sz


calc_sizes(0, -1)
components = {c: [N] for c in range(N)}
components[colors[0]] = [0]


def calc_components(node, parent):
    if parent >= 0 and colors[parent] != colors[node]:
        components[colors[parent]].append(subtree_sizes[node])
        components[colors[node]][-1] -= subtree_sizes[node]
    for i in neighbors[node]:
        if i != parent:
            calc_components(i, node)


calc_components(0, -1)

print(components)


def count_paths(n):
    return n * (n + 1) // 2


total_paths = count_paths(subtree_sizes[0])
for c in range(N):
    print(total_paths - sum(count_paths(size) for size in components[c]))
