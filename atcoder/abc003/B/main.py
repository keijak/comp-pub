import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    S = ins()
    T = ins()
    n = len(S)
    assert len(T) == n
    ok = "atcoder@"
    for i in range(n):
        if S[i] == T[i]:
            continue
        if S[i] == "@" and T[i] in ok:
            continue
        if T[i] == "@" and S[i] in ok:
            continue
        return False
    return True


print("You can win" if solve() else "You will lose")
