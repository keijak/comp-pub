import sys
from collections import deque


sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = deque(ins())
    t = deque("AKIHABARA")
    while t:
        if s and t[0] == s[0]:
            t.popleft()
            s.popleft()
        elif t[0] == "A":
            t.popleft()
        else:
            return False
    if s:
        return False
    return True


print("YES" if solve() else "NO")
