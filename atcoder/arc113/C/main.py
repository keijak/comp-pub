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
    s = ins()
    n = len(s)
    ans = 0
    c = "#"
    count = 0
    for i in range(1, n):
        if s[i] == s[i - 1] and s[i] != c:
            if c != "#":
                ans += count + (n - i)
            count = 0
            c = s[i]
        if c == "#":
            continue
        if s[i] != c:
            count += 1
    ans += count
    return ans


print(solve())

