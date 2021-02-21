import sys

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
    k = ini()
    ans = 0
    for a in range(1, k + 1):
        for b in range(1, k + 1):
            ab = a * b
            if ab > k:
                break
            ans += k // ab
    return ans


print(solve())
