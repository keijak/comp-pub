import sys
import itertools

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    v = inl()
    seen = [False] * 501
    for x in itertools.combinations(v, 3):
        seen[sum(x)] = True
    xs = [i for i, v in enumerate(seen) if v]
    xs.reverse()
    return xs[2]


print(solve())
