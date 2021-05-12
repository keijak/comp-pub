import sys
import math


def solve():
    a, b = [int(x) for x in sys.stdin.readline().split()]
    x = (a // math.gcd(a, b)) * b
    if x > 10 ** 18:
        return "Large"
    return x


print(solve())
