import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
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


def solve():
    n = ini()
    s1 = ins()
    s2 = ins()
    charset = set(s1) | set(s2)

    toi, toc = dict(), dict()
    for i in range(10):
        c = chr(ord("0") + i)
        if c in charset:
            toc[len(toi)] = c
            toi[c] = len(toi)
    for i in range(26):
        c = chr(ord("A") + i)
        if c in charset:
            toc[len(toi)] = c
            toi[c] = len(toi)

    m = len(toi)
    uf = UnionFind(m)

    for i in range(n):
        x = toi[s1[i]]
        y = toi[s2[i]]
        uf.union(x, y)

    ans = 1
    for r in uf.roots():
        is_num = False
        for y in range(10):
            c = chr(ord("0") + y)
            x = toi.get(c)
            if x is None:
                continue
            if uf.same(r, x):
                is_num = True
                break
        if is_num:
            continue
        if uf.same(r, toi[s1[0]]) or uf.same(r, toi[s2[0]]):
            ans *= 9
        else:
            ans *= 10

    return ans


print(solve())
