import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def dsum(x):
    s = 0
    while x:
        s += x % 10
        x //= 10
    return s


def solve():
    n = ini()
    a = n
    for i in range(2, 101):
        a = dsum(a)
    return a


print(solve())
