import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

S = ins()


def solve():
    N = len(S)
    i = 0
    j = 1
    flip = 0
    while j < N:
        while j < N and S[i] == S[j]:
            j += 1
        if j == N:
            break
        flip += 1
        i = j
        j += 1

    return flip


print(solve())
