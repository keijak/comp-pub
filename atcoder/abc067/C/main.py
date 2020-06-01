import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A = nl()


def solve():
    tot = sum(A)
    ans = 1 << 60
    s = 0
    for i, x in enumerate(A):
        if i == N - 1:
            break
        s += x
        a = abs(2 * s - tot)
        ans = min(ans, a)
    return ans


print(solve())
