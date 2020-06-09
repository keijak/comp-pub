import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, M = inm()
A = inl()
assert len(A) == N


def solve():
    uf = UnionFind(N)
    for _ in range(M):
        x, y = inm()
        uf.union(x, y, pay=False)
    rs = uf.roots()
    rs.sort(key=lambda x: uf.size(x), reverse=True)
    s = rs[0]
    for i in range(1, len(rs)):
        if not uf.union(s, rs[i], pay=True):
            return "Impossible"
    return uf.paid_cost()


class UnionFind:
    def __init__(self, n):
        # total number of nodes.
        self.n = n
        # node id -> root node id
        self._root_table = list(range(n))
        # root node id -> group size
        self._size_table = [1] * n
        self._cost_table = [[A[i]] for i in range(n)]  # heaps
        self._paid_cost = 0

    def paid_cost(self):
        return self._paid_cost

    def find(self, x):
        """Returns x's root node id."""
        r = self._root_table[x]
        if r != x:
            # Update the cache on query.
            r = self._root_table[x] = self.find(r)
        return r

    def union(self, x, y, pay):
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

        if pay:
            if not self._cost_table[x]:
                return False
            ax = heapq.heappop(self._cost_table[x])
            if not self._cost_table[y]:
                return False
            ay = heapq.heappop(self._cost_table[y])
            self._paid_cost += ax + ay

        for z in self._cost_table[y]:
            heapq.heappush(self._cost_table[x], z)
        self._cost_table[y] = []
        return True

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


print(solve())
