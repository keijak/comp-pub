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

MOD = 1000000007


def solve():
    n = ini()
    s = [ins() for _ in range(2)]
    assert len(s[0]) == len(s[1]) == n

    if s[0][0] == s[1][0]:
        vertical = True
        i = 1
        ans = 3
    else:
        vertical = False
        i = 2
        ans = 6

    while i < n:
        if s[0][i] == s[1][i]:
            vertical_now = True
            i += 1
            if vertical:
                ans = ans * 2 % MOD
            else:
                pass
        else:
            vertical_now = False
            i += 2
            if vertical:
                ans = ans * 2 % MOD
            else:
                ans = ans * 3 % MOD
        vertical = vertical_now

    return ans


print(solve())