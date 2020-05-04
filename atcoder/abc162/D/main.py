#!/usr/bin/env python3
import collections

N = int(input())
S = input()
tonum = {"R": 0, "G": 1, "B": 2}
ans = 0
for i in range(1, N - 1):
    d = tonum[S[i]]
    c2 = collections.Counter()
    for j in range(i + 1, N):
        c2[tonum[S[j]]] += 1
    for j in range(i):
        dj = tonum[S[j]]
        if dj == d:
            continue
        dk = 3 - d - dj
        ans += c2[dk]
        kk = i + (i - j)
        if kk < N and tonum[S[kk]] == dk:
            ans -= 1
print(ans)
