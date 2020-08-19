import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


def solve():
    n = ini()
    for x in range(n + 1):
        y = int(x * 1.08 + 1e-8)
        if y == n:
            return x
    return ":("


print(solve())
