import sys
import numpy as np
from scipy.optimize import linprog

inl = lambda: [int(x) for x in sys.stdin.readline().split()]


def solve():
    R, B = inl()
    X, Y = inl()

    c = -np.array([1, 1], dtype=np.float64)
    A_ub = np.array([[X, 1], [1, Y]], dtype=np.float64)
    b_ub = np.array([R, B], dtype=np.float64)

    lpans = linprog(c, A_ub, b_ub, method="revised simplex")

    ans = 0
    for i in range(-100, 100 + 1):
        p = int(lpans.x[0]) + i
        if p < 0:
            continue
        for j in range(-100, 100 + 1):
            q = int(lpans.x[1]) + j
            if q < 0:
                continue
            if p * X + q <= R and p + q * Y <= B:
                ans = max(ans, p + q)
    return ans


print(solve())
