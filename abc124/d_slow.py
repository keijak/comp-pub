import sys
import collections

input = sys.stdin.readline

N, K = map(int, input().split())
S = input().strip()

zerocnt = collections.Counter()
zeroindex = [-1] * N
onecnt = collections.Counter()
oneindex = [-1] * N

zh = oh = None
for i, x in enumerate(S):
    if x == "0":
        if i == 0 or S[i - 1] == "1":
            zh = i
    else:
        if i == 0 or S[i - 1] == "0":
            oh = i
    if zh is not None:
        zeroindex[i] = zh
        zerocnt[zh] += x == "0"
    if oh is not None:
        oneindex[i] = oh
        onecnt[oh] += x == "1"

dpl = [[0] * N for _ in range(K + 1)]
dpr = [[0] * N for _ in range(K + 1)]


def solve():
    dpl[0][0] = 0
    for i in range(N):
        if S[i] == "0":
            dpr[0][i] = 0
            if i > 0 and S[i - 1] == "1":
                dpl[0][i] = onecnt[oneindex[i - 1]]
        else:
            oh = oneindex[i]
            dpr[0][i] = onecnt[oh] - (i - oh)
            if i > 0 and S[i - 1] == "1":
                dpl[0][i] = i - oneindex[i - 1]
            else:
                dpl[0][i] = 0
    for k in range(1, K + 1):
        for i in range(N):
            if S[i] == "0":
                zh = zeroindex[i]
                oh = zh + zerocnt[zh]
                if oh == N:
                    dpr[k][i] = oh - i
                else:
                    dpr[k][i] = oh - i + dpr[k - 1][oh]
                if i > 0 and S[i - 1] == "1":
                    oh = oneindex[i - 1]
                    dpl[k][i] = i - oh + dpl[k - 1][oh]
                else:
                    zh = zeroindex[i]
                    dpl[k][i] = i - zh + dpl[k - 1][zh]
            else:
                oh = oneindex[i]
                zh = oh + onecnt[oh]
                if zh == N:
                    dpr[k][i] = zh - i
                else:
                    dpr[k][i] = zh - i + dpr[k - 1][zh]
                if i > 0 and S[i - 1] == "1":
                    oh = oneindex[i - 1]
                    dpl[k][i] = i - oh + dpl[k - 1][oh]
                elif i > 0:
                    zh = zeroindex[i - 1]
                    dpl[k][i] = i - zh + dpl[k - 1][zh]
    maxv = 0
    maxi = None
    maxk = None
    for i in range(N):
        for k in range(K + 1):
            v = dpl[k][i] + dpr[K - k][i]
            if maxv < v:
                maxv = v
                maxi = i
                maxk = k
    return maxv, maxi, maxk


if len(zerocnt) <= K:
    print(N)
else:
    print(solve())


print(dpl[K])
