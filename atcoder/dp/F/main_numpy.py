import numpy as np

S = np.array([ord(i) for i in input()], dtype=np.int8)
T = np.array([ord(i) for i in input()], dtype=np.int8)
LS = len(S)
LT = len(T)
equal = np.equal.outer(S, T).astype(int)

dp = [np.zeros(LT + 1, dtype=int)]
for i in range(LS):
    ndp = dp[-1].copy()
    np.maximum(ndp[:-1] + equal[i], ndp[1:], out=ndp[1:])
    np.maximum.accumulate(ndp, out=ndp)
    dp.append(ndp)


ans = []
i, j = LS, LT
while i > 0 and j > 0:
    if S[i - 1] == T[j - 1]:
        ans.append(chr(S[i - 1]))
        i -= 1
        j -= 1
    elif dp[i][j] == dp[i - 1][j]:
        i -= 1
    else:
        j -= 1
print("".join(reversed(ans)))
