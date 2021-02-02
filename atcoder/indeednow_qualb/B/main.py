import sys
from collections import deque

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = deque(ins())
    t = deque(ins())
    n = len(s)
    if len(t) != n:
        return -1
    if s == t:
        return 0
    for i in range(n):
        c = s.pop()
        s.appendleft(c)
        if s == t:
            return i + 1
    return -1


print(solve())
