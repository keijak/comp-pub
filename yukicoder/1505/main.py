import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def dump(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = int(sys.stdin.readline())
    a = [int(x) for x in sys.stdin.readline().split()]
    b = []
    for x in a:
        if b and b[-1][0] == x:
            b[-1][1] += 1
        else:
            b.append([x, 1])
    ones = 0
    for (k, v) in b:
        if k == 1:
            ones += v * (v + 1) // 2
    total = n * (n + 1) // 2
    return total - ones


print(solve())
