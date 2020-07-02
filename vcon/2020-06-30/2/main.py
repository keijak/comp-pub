import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()


def solve():
    accepted = [False] * N
    wacount = [0] * N
    for i in range(M):
        p, s = ins().split()
        p = int(p) - 1
        if s == "AC":
            accepted[p] = True
            continue

        assert s == "WA"
        if accepted[p]:
            continue
        wacount[p] += 1
    ac = wa = 0
    for i in range(N):
        if accepted[i]:
            ac += 1
            wa += wacount[i]
    return ac, wa


print(*solve())
