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
changes = []
M = ini()
for i in range(M):
    d, q = inm()
    d -= 1
    q -= 1
    changes.append((d, q))
# changes.sort()


def main():
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

    for d, after in changes:
        before = T[d]
        if before == after:
            continue
        score -= S[d][before]
        score += S[d][after]
        lb = last[before][d]
        la = last[after][d]
        da = db = 0
        for x in range(d, D):
            if x > d and T[x] == before:
                break
            last[before][x + 1] = lb
            db += 1
        score -= C[before] * (d - lb) * db
        for x in range(d, D):
            if T[x] == after:
                break
            last[after][x + 1] = d
            da += 1
        score += C[after] * (d - la) * da
        T[d] = after
        print(score)


main()
