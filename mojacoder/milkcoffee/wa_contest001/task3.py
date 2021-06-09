import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def f(L, y, x, z):
    h = x + z - y
    if h <= L:
        return 4
    if x == z == L:
        return 3
    if x == L or z == L:
        return 4
    return 5


def solve():
    L, X, Y, Z = [int(x) for x in sys.stdin.readline().split()]
    if min(X, Z) <= Y <= max(X, Z):
        return 4
    if Y < min(X, Z):
        return f(L, Y, X, Z)
    assert Y > max(X, Z)
    return f(L, L - Y, L - X, L - Z)


print(solve())
