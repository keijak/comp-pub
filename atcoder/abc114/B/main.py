import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    n = len(s)
    ans = 10 ** 5
    for i in range(n - 2):
        x = int(s[i : i + 3])
        ans = min(ans, abs(753 - x))
    return ans


print(solve())
