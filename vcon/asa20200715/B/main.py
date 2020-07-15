import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = list(ins())


def solve():
    n = len(S)
    k = n
    i = 0
    while i < n:
        if i + 1 < n and S[i] == S[i + 1]:
            k -= 1
            i += 3
        else:
            i += 1
    return k


print(solve())
