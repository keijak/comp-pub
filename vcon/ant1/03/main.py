import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

K, S = inm()


def solve():
    count = 0
    for x in range(K + 1):
        for y in range(K + 1):
            z = S - x - y
            if 0 <= z <= K:
                count += 1
    return count


print(solve())
