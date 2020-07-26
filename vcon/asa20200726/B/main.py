import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


S = ins()


def is_even(s):
    if len(s) % 2 == 1:
        return False
    k = len(s) // 2
    for i in range(k):
        if s[i] != s[k + i]:
            return False
    return True


def solve():
    for i in range(len(S) - 1, 1, -1):
        s = S[:i]
        if is_even(s):
            return i
    return None


print(solve())
