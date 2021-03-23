import sys
import math


inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def f(a, h):
    u = min(a, h - a)
    ret = a * u ** 2 - u ** 3 / 3
    if a < h - a:
        ret += a ** 2 * (h - 2 * a)
    ret /= (h - a) ** 2
    return ret


def solve():
    h, w, a, b = inl()
    return f(a, h) * f(b, w)


print(solve())
