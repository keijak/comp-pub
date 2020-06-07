import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N, M, S = inm()
S -= 1
adj = [[] for _ in range(N)]
for i in range(M):
    u, v = inm()
    u -= 1
    v -= 1
    adj[u].append(v)
    adj[v].append(u)


def solve():
    active = [False] * N

    def activate(i, lb):
        active[i] = True
        for j in adj[i]:
            if j > lb and not active[j]:
                activate(j, lb)

    ans = []
    for i in range(N - 1, -1, -1):
        if i == S:
            ans.append(i + 1)
            activate(i, i)
            continue
        for j in adj[i]:
            if j > i and active[j]:
                ans.append(i + 1)
                activate(i, i)
                break

    print(*reversed(ans), sep="\n")


solve()
