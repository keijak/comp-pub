import sys
from fractions import Fraction

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a, b = inl()
    if a < b:
        a, b = b, a
    if a == 1:
        return 0
    pr = a * b

    fv, tv = 0, a - 1
    while tv - fv > 1:
        mid = (tv + fv) // 2
        if mid == 0:
            fv = mid
            continue
        y1 = Fraction(pr - 1, mid)
        y2 = Fraction(pr - 1, mid + 1)
        if y1 - y2 <= 1:
            tv = mid
        else:
            fv = mid
    ty2 = (pr - 1) // tv
    debug(f"at {tv}, {ty2}")
    ans = tv + ty2 - 2

    return ans


def main():
    q = ini()
    for i in range(q):
        print(solve())


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    main()
