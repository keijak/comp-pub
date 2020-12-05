import sys
import math

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    x = 1
    for i in range(2, n + 1):
        g = math.gcd(x, i)
        x *= i // g
    return x + 1


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())