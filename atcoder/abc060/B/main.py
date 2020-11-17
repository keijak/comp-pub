import sys
import math

inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def solve():
    a, b, c = inl()
    return c % math.gcd(a, b) == 0


print("YES" if solve() else "NO")

