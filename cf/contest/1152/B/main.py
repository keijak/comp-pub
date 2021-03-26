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


def is_pow2(x):
    while (x & 1) == 0:
        x >>= 1
    return x == 1


def solve():
    x = ini()
    a = []
    while True:
        s = bin(x)[2:]
        i = s.find("0")
        if i < 0:
            return a
        suffix = s[i:]
        if suffix == "0" * len(suffix):
            d = len(s) - i - 1
        else:
            d = len(s) - i
        x ^= 2 ** d - 1
        x += 1
        a.append(d)


a = solve()
print(len(a) * 2)
print(*a)
