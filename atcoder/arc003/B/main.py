import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    words = [None] * n
    for i in range(n):
        w = ins()
        r = w[::-1]
        words[i] = (r, w)
    words.sort()
    for (r, w) in words:
        print(w)


solve()
