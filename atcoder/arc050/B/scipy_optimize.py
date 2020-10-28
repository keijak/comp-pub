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

    res = linprog(c, A_ub, b_ub, method="revised simplex")
    px, py = res.x
    qx, qy = int(px + 0.5), int(py + 0.5)
    return qx + qy


print(solve())
