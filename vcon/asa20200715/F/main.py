import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


class Dinic:
    def __init__(self, n, g):
        self.n = n
        self.e = [[] for _ in range(n)]
        for i in range(n):
            for j, c in g[i]:
                a = [j, c, None]
                a[2] = b = [i, 0, a]
                self.e[i].append(a)
                self.e[j].append(b)

    def bfs(self, s, t):
        self.d = d = [None] * self.n
        d[s] = 0
        k = [s]
        e = self.e
        for i in k:
            for j, c, _ in e[i]:
                if c and d[j] == None:
                    d[j] = d[i] + 1
                    k.append(j)
        return d[t] != None

    def dfs(self, v, t, f):
        if v == t:
            return f
        d = self.d
        for e in self.m[v]:
            w, c, r = e
            if c and d[v] < d[w]:
                b = self.dfs(w, t, min(f, c))
                if b:
                    e[1] -= b
                    r[1] += b
                    return b
        return 0

    def flow(self, s, t):
        l = 0
        x = 10 ** 20
        while self.bfs(s, t):
            self.m = list(map(iter, self.e))
            f = x
            while f:
                f = self.dfs(s, t, x)
                l += f
        return l


N, girls, E = inm()
P = inl()
G = [[] for _ in range(N + 1)]
for i in range(E):
    a, b = inm()
    G[a].append((b, 1))
    G[b].append((a, 1))
for p in P:
    G[p].append((N, 1))

dinic = Dinic(N + 1, G)


def solve():
    return dinic.flow(0, N)


print(solve())
