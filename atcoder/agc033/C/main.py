import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
adj = [[] for _ in range(N)]

for i in range(N - 1):
    a, b = [x - 1 for x in inm()]
    adj[a].append(b)
    adj[b].append(a)

depth = [(0, 0) for _ in range(N)]


def solve():
    stk = [(0, -1, False)]
    while stk:
        node, p, agg = stk.pop()
        if not agg:
            stk.append((node, p, True))
        md1, md2 = 0, 0
        for x in adj[node]:
            if x == p:
                continue
            if agg:
                dx, _ = depth[x]
                dx += 1
                if md1 < dx:
                    md2 = md1
                    md1 = dx
                elif md2 < dx:
                    md2 = dx
            else:
                stk.append((x, node, False))
        if agg:
            depth[node] = (md1, md2)

    stk = [(0, -1)]
    max_len = 0
    while stk:
        node, p = stk.pop()
        d1, d2 = depth[node]
        if p >= 0:
            pd1, pd2 = depth[p]
            if pd1 == 1 + d1:
                pd1 = pd2
            if d2 < pd1 + 1:
                d2 = pd1 + 1
        if max_len < d1 + d2 + 1:
            max_len = d1 + d2 + 1
        for x in adj[node]:
            if x != p:
                stk.append((x, node))

    return (max_len + 1) % 3 != 0


print("First" if solve() else "Second")
