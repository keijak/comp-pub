import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n, m = inm()
sc = [tuple(inm()) for i in range(m)]


def digits(x):
    if x < 10:
        return 1
    if x < 100:
        return 2
    return 3


def solve():
    for x in range(1000):
        if digits(x) != n:
            continue
        sx = str(x)
        ok = True
        for i in range(m):
            s, c = sc[i]
            if sx[s - 1] != str(c):
                ok = False
                break
        if ok:
            return x

    return -1


print(solve())
