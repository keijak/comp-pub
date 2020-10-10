import sys
from collections import deque

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    s = ins()
    assert len(s) == n
    if n % 2 == 0:
        return -1

    d = deque()
    d.append("b")
    i = 1
    while len(d) < n:
        if i % 3 == 1:
            d.appendleft("a")
            d.append("c")
        elif i % 3 == 2:
            d.appendleft("c")
            d.append("a")
        else:
            d.appendleft("b")
            d.append("b")
        i += 1

    if "".join(d) != s:
        return -1

    return i - 1


print(solve())
