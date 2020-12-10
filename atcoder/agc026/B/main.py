import sys
import math

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b, c, d = inl()
    if a < b or d < b:
        return False
    if c >= b - 1:
        return True
    x1 = a % b
    if c < x1:
        return False
    g = math.gcd(d, b)
    k = (b - x1 + (g - 1)) // g - 1
    mx = x1 + k * g
    return mx <= c


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    t = ini()
    for i in range(t):
        r = solve()
        print("Yes" if r else "No")
