import sys
import numpy as np
import numba

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
P = np.array([np.float64(p) for p in readline().split()], dtype=np.float64)


@numba.njit(numba.f8(numba.i8, numba.typeof(P)))
def solve(N, P):
    dp = np.zeros(N + 1, dtype=np.float64)
    dp[0] = 1.0 - P[0]
    dp[1] = P[0]
    for i in range(1, N):
        p = P[i]
        q = np.zeros_like(dp)
        q[1:] = dp[:-1]
        dp = dp * (1.0 - p) + q * p
    return dp[N // 2 + 1 :].sum()


if __name__ == "__main__":
    print(solve(N, P))
