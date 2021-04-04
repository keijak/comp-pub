import sys


def solve():
    n, a, b = [int(x) for x in sys.stdin.readline().split()]
    if (b - a - 1) % 2 == 1:
        return "Alice"
    else:
        return "Borys"


print(solve())