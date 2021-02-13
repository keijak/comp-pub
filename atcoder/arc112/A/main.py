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


def f(r, s):
    return s * (r - s - 1)


def solve(l, r):
    if 2 * l > r:
        return 0
    ans = r // 2 - l + 1
    ans += 2 * (f(r, r // 2) - f(r, l - 1))
    return ans


T = ini()
for i in range(T):
    l, r = inl()
    print(solve(l, r))
