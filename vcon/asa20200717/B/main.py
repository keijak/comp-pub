import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()
st = []
for i in range(N):
    a, b = inm()
    st.append((a, b))
cp = []
for i in range(M):
    c, d = inm()
    cp.append((c, d))


def solve():
    for i in range(N):
        a, b = st[i]
        mindis = 10 ** 12
        mcp = None
        for j in range(M):
            c, d = cp[j]
            dis = abs(a - c) + abs(b - d)
            if mindis > dis:
                mindis = dis
                mcp = j + 1
        print(mcp)


solve()
