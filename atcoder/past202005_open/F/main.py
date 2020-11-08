import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
mat = [None] * N
for i in range(N):
    mat[i] = set(ins())


def solve():
    i = 0
    j = N - 1
    pal = [None] * N
    while i < j:
        si, sj = mat[i], mat[j]
        z = si & sj
        if not z:
            return -1
        c = next(iter(z))
        assert isinstance(c, str)
        assert len(c) == 1
        pal[i] = pal[j] = c
        i += 1
        j -= 1
    if N % 2 == 1:
        assert i == j
        c = next(iter(mat[i]))
        pal[i] = c
    return "".join(pal)


print(solve())
