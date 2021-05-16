import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = int(sys.stdin.readline())
    b = 0
    bb = 1
    ans = n
    while bb <= n:
        a = n // bb
        c = n - a * bb
        assert c >= 0
        ans = min(ans, a + b + c)
        b += 1
        bb *= 2
    return ans


print(solve())
