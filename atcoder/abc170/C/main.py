import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

X, N = inm()
P = inl()


def solve():
    ps = set(P)
    if X not in ps:
        return X

    mv = 1000
    my = 10000
    for y in range(-1, 102):
        if y in ps:
            continue
        if abs(y - X) < mv or abs(y - X) == mv and y < my:
            mv = abs(y - X)
            my = y
    return my


print(solve())
