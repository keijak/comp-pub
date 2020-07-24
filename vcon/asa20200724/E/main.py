import sys
from collections import defaultdict

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


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


n, m = inm()
INF = 10 ** 12


def solve():
    pos = [None] * n
    uf = UnionFind(n)
    for i in range(m):
        l, r, d = inm()
        l -= 1
        r -= 1
        uf.union(l, r)
        if pos[l] is None and pos[r] is None:
            pos[l] = 0
            pos[r] = d
        elif pos[r] is None:
            pos[r] = pos[l] + d
        elif pos[l] is None:
            pos[l] = pos[r] - d
        elif pos[r] - pos[l] != d:
            return False
    mins = defaultdict(lambda: INF)
    maxs = defaultdict(lambda: -INF)

    for i in range(n):
        r = uf.find(i)
        if r != i:
            mins[r] = min(mins[r], pos[i])
            maxs[r] = max(maxs[r], pos[i])

    for r in mins.keys():
        if maxs[r] - mins[r] > 10 ** 9:
            return False
    return True


print("Yes" if solve() else "No")
