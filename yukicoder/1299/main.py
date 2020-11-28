import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

M = 998244353


def solve():
    n, k = inl()
    a = inl()
    s = sum(a) % M
    return s * pow(2, k, M) % M


if __name__ == "__main__":
    print(solve())