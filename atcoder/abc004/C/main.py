import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    n %= 30
    ans = [i + 1 for i in range(6)]
    for j in range(n):
        i = j % 5
        ans[i], ans[i + 1] = ans[i + 1], ans[i]
    return ans


print(*solve(), sep="")
