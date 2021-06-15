import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, c = [int(x) for x in sys.stdin.readline().split()]
    if c % 2 == 0:
        a = abs(a)
        b = abs(b)
    if a == b:
        return "="
    elif a < b:
        return "<"
    else:
        return ">"


print(solve())
