import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


def solve():
    x = ini()
    lb, ub = 0, 0
    while lb <= x:
        if lb <= x <= ub:
            return True
        lb += 100
        ub += 105
    return False


print(1 if solve() else 0)
