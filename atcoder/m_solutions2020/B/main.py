import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    A, B, C = inm()
    K = ini()
    while B <= A:
        B *= 2
        K -= 1
    while C <= B:
        C *= 2
        K -= 1
    if K < 0:
        return False
    return True


print("Yes" if solve() else "No")

