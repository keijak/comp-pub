import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

D = ini()
C = inl()
assert len(C) == 26
S = [None] * D
for i in range(D):
    S[i] = inl()
    assert len(S[i]) == 26
T = [ini() - 1 for _ in range(D)]


def solve():
    last = [[-1] * (D + 1) for _ in range(26)]
    score = 0
    for d in range(D):
        ct = T[d]
        score += S[d][ct]
        for i in range(26):
            last[i][d + 1] = last[i][d]
        last[ct][d + 1] = d
        for i in range(26):
            score -= C[i] * (d - last[i][d + 1])
        print(score)


solve()
