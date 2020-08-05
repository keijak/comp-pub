import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = ins()
a = inl()


def solve():
    p = 0
    for x in a:
        print(S[p:x] + '"', end="")
        p = x
    print(S[p:])


solve()
