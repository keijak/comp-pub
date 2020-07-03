import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = ins()
K = "keyence"


def solve():
    if len(S) < len(K):
        return False
    f = b = True
    x = y = 0
    ns = len(S)
    nk = len(K)
    for i in range(len(S)):
        if i < nk and S[i] == K[i]:
            if f:
                x += 1
        else:
            f = False
        if nk - 1 - i >= 0 and S[ns - 1 - i] == K[nk - 1 - i]:
            if b:
                y += 1
        else:
            b = False
    # debug(f"{x=} {y=}")
    return x + y >= nk


print("YES" if solve() else "NO")
