import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = ins()


def solve():
    ans = 0
    n = len(S)
    for i, x in enumerate(S):
        if x == "U":
            ans += n - 1 - i
            ans += 2 * i
        else:
            ans += 2 * (n - 1 - i)
            ans += i

    return ans


print(solve())
