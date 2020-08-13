import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
S, T = ins().split()
assert len(S) == N
assert len(T) == N


def solve():
    res = []
    for i in range(N):
        res.append(S[i])
        res.append(T[i])
    return "".join(res)


print(solve())
