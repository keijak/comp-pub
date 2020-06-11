import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

S = [int(x == "1") for x in ins()]
N = len(S)


def check(k):
    assert k < N
    if 2 * k - 1 <= N:
        return True
    a = S[k - 1]
    return all(S[i] == a for i in range(N - k, k))


def solve():
    if all(x == 0 for x in S):
        return N
    if all(x == 1 for x in S):
        return N

    ok, ng = 1, N
    while ng - ok > 1:
        m = (ok + ng) // 2
        if check(m):
            ok = m
        else:
            ng = m
    return ok


print(solve())
