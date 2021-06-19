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

    x = (108 * n) // 100
    if x < 206:
        return "Yay!"
    elif x == 206:
        return "so-so"
    return ":("


print(solve())
