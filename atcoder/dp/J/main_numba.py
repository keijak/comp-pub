import collections
import sys
import numpy as np
import numba

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
A = [int(x) for x in readline().split()]
ac = collections.Counter(A)
assert set(ac.keys()).issubset({1, 2, 3})
C1, C2, C3 = ac[1], ac[2], ac[3]


@numba.njit("f8(i4,i4,i4)", cache=True)
def solve(c1, c2, c3):
    K = c1 + c2 + c3
    dp = np.zeros((K + 3, c2 + c3 + 2, c3 + 1), dtype=np.float64)
    for i3 in range(c3 + 1):
        for i2 in range(c2 + c3 + 1):
            for i1 in range(N - i2 - i3 + 1):
                isum = i1 + i2 + i3
                if isum == 0:
                    continue
                i0 = N - i1 - i2 - i3
                if i0 < 0:
                    continue
                e = i0
                if i1 > 0:
                    e += i1 * (1.0 + dp[i1 - 1, i2, i3])
                if i2 > 0 and i1 <= K:
                    e += i2 * (1.0 + dp[i1 + 1, i2 - 1, i3])
                if i3 > 0 and i2 + 1 <= c2 + c3:
                    e += i3 * (1.0 + dp[i1, i2 + 1, i3 - 1])
                dp[i1, i2, i3] = e / isum
    return dp[c1, c2, c3]


if __name__ == "__main__":
    print(solve(C1, C2, C3))
