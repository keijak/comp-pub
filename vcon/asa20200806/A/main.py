import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = ins()
a = inl()
assert len(a) == 4


def solve():
    n = len(S)
    j = 0
    for i in range(n + 1):
        if j < 4 and i == a[j]:
            print('"', end="")
            j += 1
        if i < n:
            print(S[i], end="")
    print()


solve()
