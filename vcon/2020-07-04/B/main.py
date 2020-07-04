import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, M = inm()


def solve():
    pos = {0}
    count = [1] * N
    for i in range(M):
        x, y = inm()
        x -= 1
        y -= 1
        if x in pos:
            if count[x] == 1:
                pos.remove(x)
            pos.add(y)
        count[x] -= 1
        count[y] += 1
    return len(pos)


print(solve())
