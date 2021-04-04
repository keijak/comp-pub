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


def parin(s):
    n = len(s)
    for i in range(n // 2):
        if s[i] != s[n - 1 - i]:
            return False
    return True


def solve():
    n = ini()
    cnt = 0
    for i in range(1, n + 1):
        s = str(i)
        if len(s) % 2 == 1:
            continue
        if parin(s):
            cnt += 1
    return cnt


print(solve())
