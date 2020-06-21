import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()


def solve():
    p = a = 26
    l = 1
    while a < N:
        p *= 26
        a += p
        l += 1
    x = N - 1 - (a - p)
    name = [None] * l
    for i in range(l):
        c = x % 26
        x //= 26
        name[i] = chr(ord("a") + c)
    return "".join(name[::-1])


print(solve())
