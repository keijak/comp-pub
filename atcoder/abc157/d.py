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


class UnionFind:
    def __init__(self, n):
        # total number of nodes.
        self.n = n
        # node id -> root node id
        self._root_table = list(range(n))
        # root node id -> group size
        self._size_table = [1] * n

    def find(self, x):
        """Returns x's root node id."""
        r = self._root_table[x]
        if r != x:
            # Update the cache on query.
            r = self._root_table[x] = self.find(r)
        return r

    def union(self, x, y):
        """Merges two groups."""
        x = self.find(x)
        y = self.find(y)

        if x == y:
            return

        # Ensure that x is the larger (or equal) group.
        if self._size_table[x] < self._size_table[y]:
            x, y = y, x

        self._size_table[x] += self._size_table[y]
        self._root_table[y] = x

    def size(self, x):
        return self._size_table[self.find(x)]

    def same(self, x, y):
        return self.find(x) == self.find(y)

    def members(self, x):
        root = self.find(x)
        return [i for i in range(self.n) if self.find(i) == root]

    def roots(self):
        return [i for i, x in enumerate(self._root_table) if x == i]

    def group_count(self):
        return len(self.roots())

    def all_group_members(self):
        return {r: self.members(r) for r in self.roots()}

    def __str__(self):
        return "\n".join("{}: {}".format(r, self.members(r)) for r in self.roots())


uf = UnionFind(N)
for i in range(N):
    for j in friends[i]:
        uf.union(i, j)

ans = []
for i in range(N):
    res = uf.size(i) - len(friends[i]) - 1
    for b in blocked[i]:
        if uf.same(i, b):
            res -= 1
    ans.append(res)
print(*ans)
