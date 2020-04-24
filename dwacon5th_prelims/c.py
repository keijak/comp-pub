import sys

input = sys.stdin.readline
N = int(input())
S = input().rstrip()
Q = int(input())
K = list(map(int, input().split()))

ds = [0] * N
ms = [0] * N
dms = [0] * N
cpos = []
for i, c in enumerate(S):
    if i > 0:
        ds[i] += ds[i - 1]
        ms[i] += ms[i - 1]
        dms[i] += dms[i - 1]
    if c == "D":
        ds[i] += 1
    elif c == "M":
        ms[i] += 1
    elif c == "C" and i > 0 and dms[i - 1] > 0:
        cpos.append(i)
    if i > 0 and c == "M":
        dms[i] += ds[i - 1]

for k in K:
    ans = 0
    for i in cpos:
        ans += dms[i - 1]
        if i - k >= 0:
            ans -= dms[i - k] + (ms[i - 1] - ms[i - k]) * ds[i - k]
    print(ans)
