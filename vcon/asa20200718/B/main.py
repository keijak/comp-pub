import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

v = inl()
v.sort()


def solve():
    k1 = (v[2] - v[0]) // 2
    v[0] += 2 * k1
    k2 = (v[2] - v[1]) // 2
    v[1] += 2 * k2
    if v[0] == v[1] == v[2]:
        return k1 + k2
    elif v[0] == v[1] == (v[2] - 1):
        return k1 + k2 + 1
    else:
        return k1 + k2 + 2


print(solve())
