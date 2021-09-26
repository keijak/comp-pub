import numpy as np

p, n = input().split()
p, n = np.float128(p), int(n)
P = np.array([[1.0 - p, p], [p, 1.0 - p]], dtype=np.float128)
v = np.array([0.0, 1.0], dtype=np.float128)
ans = np.matmul(P ** n, v)
print(ans)


# pon, poff = p, 1 - p
# for i in range(1, n):
#     pon, poff = (pon * (1 - p) + poff * p), (pon * p + poff * (1 - p))
# print(pon)
