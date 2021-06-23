import sys


def solve():
    n, m = [int(x) for x in sys.stdin.readline().split()]
    if m == 1:
        return 2
    if n % 2 == 0:
        return 2
    return 1


t = int(sys.stdin.readline())
for i in range(t):
    print(solve())
