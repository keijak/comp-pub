import collections
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
A = [int(x) for x in readline().split()]
ac = collections.Counter(A)
assert set(ac.keys()).issubset({1, 2, 3})
K = sum(ac.values())


dp = [[[None] * (K + 1) for _ in range(K + 1)] for _ in range(K + 1)]


def solve(c1, c2, c3):
    csum = c1 + c2 + c3
    if csum == 0:
        return 0.0
    cached = dp[c1][c2][c3]
    if cached is not None:
        return cached
    ans = 0.0
    if c1 > 0:
        ans += c1 / csum * (1.0 + solve(c1 - 1, c2, c3))
    if c2 > 0:
        ans += c2 / csum * (1.0 + solve(c1 + 1, c2 - 1, c3))
    if c3 > 0:
        ans += c3 / csum * (1.0 + solve(c1, c2 + 1, c3 - 1))
    c0 = N - csum
    ans += c0 / csum
    dp[c1][c2][c3] = ans
    return ans


if __name__ == "__main__":
    print(solve(ac[1], ac[2], ac[3]))
