import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

S, K = ins().split()
K = int(K)


def dec():
    x = 0
    for d in S:
        x <<= 1
        if d == "1":
            x |= 1
    return x


def solve():
    bits = dec()
    return f"{bits:0b}"


print(solve())
