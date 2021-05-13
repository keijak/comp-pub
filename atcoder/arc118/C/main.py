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
    a = [3 * 5 * 7 * 11]
    for x in range(6, 10001):
        if len(a) == n:
            break
        if x % 2 != 0:
            continue
        if x % 3 == 0 or x % 5 == 0 or x % 7 == 0 or x % 11 == 0:
            a.append(x)
    return a


print(*solve())
