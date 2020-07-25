import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

A, B = inm()


def solve():
    da = A // 100
    if da < 9:
        moda = 900 + A % 100
    else:
        da = (A % 100) // 10
        if da < 9:
            moda = A - da * 10 + 90
        else:
            moda = A - A % 10 + 9
    db = B // 100
    if db > 1:
        modb = 100 + B % 100
    else:
        db = (B % 100) // 10
        if db > 0:
            modb = B - db * 10
        else:
            modb = B - B % 10

    return max(A - modb, moda - B)


print(solve())
