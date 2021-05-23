import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def f(ch):
    d = {
        "0": "0",
        "1": "1",
        "6": "9",
        "8": "8",
        "9": "6",
    }
    return d[ch]


def solve():
    s = sys.stdin.readline().rstrip()
    s = reversed(s)
    t = map(f, s)
    return "".join(t)


print(solve())
