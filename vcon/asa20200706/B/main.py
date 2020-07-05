import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
d = []
for i in range(N):
    s, p = ins().split()
    p = int(p)
    d.append((s, -p, i + 1))
d.sort()


def solve():
    for _, _, i in d:
        print(i)


solve()
