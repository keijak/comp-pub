#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
S = ns()
assert len(S) == N
Q = ni()
qs = []
for i in range(Q):
    q = ns().split()
    qs.append(q)


def main():
    orda = ord("a")
    ords = [ord(ch) - orda for ch in S]
    bits = [BIT(N) for _ in range(26)]
    for i, ch in enumerate(ords):
        bits[ch].add(i + 1, +1)  # 1-origin
    for q in qs:
        if q[0] == "1":
            j = int(q[1])  # 1-origin
            nch = ord(q[2]) - orda
            och = ords[j - 1]
            if nch != och:
                ords[j - 1] = nch
                bits[och].add(j, -1)
                bits[nch].add(j, +1)
        else:
            assert q[0] == "2"
            l, r = int(q[1]), int(q[2])  # 1-origin
            n = 0
            for i in range(26):
                b = bits[i]
                if b.sum(r) - b.sum(l - 1) > 0:
                    n += 1
            print(n)


# Binary Indexed Tree (Fenwick Tree)
# 1-origin index.
class BIT:
    def __init__(self, n):
        self.size = n
        self.tree = [0] * (n + 1)

    def sum(self, i):
        s = 0
        while i > 0:
            s += self.tree[i]
            i -= i & -i
        return s

    def add(self, i, x):
        assert i > 0
        while i <= self.size:
            self.tree[i] += x
            i += i & -i


main()
