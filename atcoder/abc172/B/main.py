import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

s = ins()
t = ins()


def solve():
    n = len(s)
    cnt = 0
    for i in range(n):
        if s[i] != t[i]:
            cnt += 1
    return cnt


print(solve())
