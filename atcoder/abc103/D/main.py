import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **dict(file=sys.stderr, **kw))

N, M = inm()


def solve():
    xs = []
    for i in range(M):
        a, b = inm()
        xs.append((a, True, i))
        xs.append((b, False, i))
    xs.sort()
    cut = 0
    opn = set()
    closed = set()
    for x, start, i in xs:
        if start:
            opn.add(i)
        elif i in closed:
            closed.remove(i)
        else:
            cut += 1
            opn.remove(i)
            closed |= opn
            opn = set()
    assert not opn
    assert not closed
    return cut


print(solve())
