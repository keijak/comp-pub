import sys
import math


def solve():
    h, r = map(float, input().split())
    S = -8.245 + 6.807 * h + 7.073 * math.pi * 2 * r
    return S


print(solve())