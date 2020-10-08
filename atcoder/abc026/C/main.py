import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    child = [[] for _ in range(n)]
    for i in range(n - 1):
        b = ini()
        b -= 1
        child[b].append(i + 1)

    def salary(v):
        xs = []
        for u in child[v]:
            xs.append(salary(u))
        if not xs:
            return 1
        return max(xs) + min(xs) + 1

    return salary(0)


print(solve())
