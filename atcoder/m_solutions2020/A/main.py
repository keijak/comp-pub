import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


X = ini()


def solve():
    if X < 600:
        return 8
    if X < 800:
        return 7
    if X < 1000:
        return 6
    if X < 1200:
        return 5
    if X < 1400:
        return 4
    if X < 1600:
        return 3
    if X < 1800:
        return 2
    return 1


print(solve())
