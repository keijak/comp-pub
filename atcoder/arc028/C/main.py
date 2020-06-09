import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
adj = [[] for _ in range(N)]
for i in range(1, N):
    p = ini()
    adj[i].append(p)
    adj[p].append(i)


def solve():
    size = [0 for _ in range(N)]
    maxcsize = [0 for _ in range(N)]

    def dfs1(node, p):
        sz = 1
        mc = 0
        for x in adj[node]:
            if x == p:
                continue
            z = dfs1(x, node)
            mc = max(mc, z)
            sz += z
        maxcsize[node] = mc
        size[node] = sz
        return sz

    dfs1(0, -1)
    ans = [None] * N

    def dfs2(node, p, psize):
        ans[node] = max(maxcsize[node], psize)
        psize += size[node]
        for x in adj[node]:
            if x == p:
                continue
            dfs2(x, node, psize - size[x])

    dfs2(0, -1, 0)
    return ans


print(*solve(), sep="\n")
