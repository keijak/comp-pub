import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
K = ni()


def solve():
    x = 1
    for i in range(N):
        x = min(2 * x, x + K)
    return x


print(solve())
