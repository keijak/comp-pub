import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


n = ini()
res = []
for i in range(n):
    s, p = ins().split()
    p = int(p)
    res.append((s, -p, i))
res.sort()


def solve():
    return [i + 1 for s, p, i in res]


print(*solve(), sep="\n")
