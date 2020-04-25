from collections import Counter


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


H, W = map(int, map(int, input().split()))
grid = []
for i in range(H):
    grid.append(input())
uf = UnionFind(H * W)
for i in range(H):
    for j in range(W):
        nid = i * W + j
        c = grid[i][j]
        if i > 0 and grid[i - 1][j] != c:
            uf.union(nid, (i - 1) * W + j)
        if i < H - 1 and grid[i + 1][j] != c:
            uf.union(nid, (i + 1) * W + j)
        if j > 0 and grid[i][j - 1] != c:
            uf.union(nid, i * W + (j - 1))
        if j < W - 1 and grid[i][j + 1] != c:
            uf.union(nid, i * W + (j + 1))
whites = Counter()
for i in range(H):
    for j in range(W):
        if grid[i][j] == ".":
            whites[uf.find(i * W + j)] += 1
ans = 0
for i in range(H):
    for j in range(W):
        if grid[i][j] == "#":
            ans += whites[uf.find(i * W + j)]
print(ans)
