import sys

M = 2019
S = sys.stdin.readline().rstrip()
N = len(S)

dmod = [0] * (N + 1)
amod = [0] * (N + 1)
tenmod = 1
prev_amod = 0
mcnt = [0] * 2019
ans = 0
for i in range(N, -1, -1):
    d = ord(S[i]) - ord("0") if i < N else 0
    dmod[i] = d * tenmod % M
    amod[i] = (dmod[i] + prev_amod) % M
    # print(f"{i}\t{d} * {tenmod} is {dmod[i]} : {amod[i]} mcnt={mcnt[amod[i]]}")

    ans += mcnt[amod[i]]
    mcnt[amod[i]] += 1
    tenmod = tenmod * 10 % M
    prev_amod = amod[i]
print(ans)
