import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n, m = inm()
a = inl()


def solve():
    g = []
    for i in range(m):
        b, c = inm()
        g.append((c, b))
    g.sort(reverse=True)
    h = []
    for c, b in g:
        if len(h) == n:
            break
        for i in range(b):
            h.append(c)
            if len(h) == n:
                break
    assert len(h) <= n

    heapq.heapify(a)
    for x in h:
        if a[0] >= x:
            break
        heapq.heappush(a, x)
        heapq.heappop(a)
    return sum(a)


print(solve())
