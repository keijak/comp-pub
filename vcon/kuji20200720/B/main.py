import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
w = [ins() for _ in range(n)]


def solve():
    p = set()
    p.add(w[0])
    for i in range(1, n):
        if w[i] in p:
            return False
        if w[i - 1][-1] != w[i][0]:
            return False
        p.add(w[i])
    return True


print("Yes" if solve() else "No")
