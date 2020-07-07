class SegTree:
    unity = 1 << 50
    func = min

    def __init__(self, n):
        # size
        self.n = n

        # Internally, we store values in 1-based index.
        # `value[1]` stores the root value.
        # Leaf values are stored in [n, 2*n).
        # Forall x, x's children are (x<<1) and (x<<1 | 1).
        self.values = [SegTree.unity] * (2 * n)

    def build(self, seq):
        """Initializes the segment tree with a given sequence."""
        for i, x in enumerate(seq, self.n):
            self.values[i] = x
        for i in range(self.n - 1, 0, -1):
            self.values[i] = self.func(self.values[i << 1], self.values[i << 1 | 1])

    def __setitem__(self, i, x):
        """Sets i-th value (0-indexed) to x."""
        i += self.n
        self.values[i] = x
        while i > 1:
            i >>= 1
            self.values[i] = self.func(self.values[i << 1], self.values[i << 1 | 1])

    def fold(self, l, r):
        """Queries by [l, r) range (0-indexed, open interval)."""
        l += self.n
        r += self.n
        lval = SegTree.unity
        rval = SegTree.unity
        while l < r:
            if l & 1:
                lval = SegTree.func(lval, self.values[l])
                l += 1
            if r & 1:
                r -= 1
                rval = SegTree.func(self.values[r], rval)
            l >>= 1
            r >>= 1
        return SegTree.func(lval, rval)
