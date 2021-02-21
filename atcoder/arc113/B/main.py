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


# phi(10) = 4
# a^(b^c) = (2^x 5^y z)^(b^c) = 2^(x b^c) 5^(y b^c) z^(b^c)


def solve():
    a, b, c = inl()
    a %= 10

    d = {}
    x = 1
    offset = -1
    cycle = 0
    for i in range(20):
        x *= a
        x %= 10
        debug(f"{i=} {x=}")
        if x in d:
            j = d[x]
            cycle = i - j
            offset = j
            break
        else:
            d[x] = i

    debug(f"{cycle=} {offset=}")
    assert offset == 0
    x = pow(b, c, cycle)
    if x == 0:
        x = cycle
    ans = pow(a, x, 10)
    debug(f"{x=} {ans=}")
    return ans


print(solve())
