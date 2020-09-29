import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inl()
A = inl()
A.sort(reverse=True)

ps = []
for i in range(N):
    ps.append((2 * A[i], A[i], A[i]))
    for j in range(i + 1, N):
        ps.append((A[i] + A[j], A[i], A[j]))
        ps.append((A[i] + A[j], A[j], A[i]))
ps.sort(reverse=True)

debug(ps[M - 1])

ans = 0
for v, l, r in ps[:M]:
    ans += v
print(ans)
