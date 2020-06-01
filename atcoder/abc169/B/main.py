import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A = nl()
L = 10 ** 18


def solve():
    ans = 1
    for a in A:
        if a == 0:
            ans = 0
            break
        if ans > 0:
            ans *= a
        if ans > L:
            ans = -1
    return ans


print(solve())
