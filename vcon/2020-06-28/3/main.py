import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()
A = inl()
ac = collections.Counter(A)


def solve():
    v, c = ac.most_common(1)[0]
    if 2 * c > N:
        return v
    return "?"


print(solve())
