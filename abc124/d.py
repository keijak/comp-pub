import sys
import collections

input = sys.stdin.readline

N, K = map(int, input().split())
S = input().strip()

zerocnt = collections.Counter()
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
        zerocnt[zh] += x == "0"
    if oh is not None:
        oneindex[i] = oh

zerolist = sorted(zerocnt.keys()) + [N]


def solve():
    maxval = -1
    maxrange = None
    for i, end in enumerate(zerolist):
        zstart = max(i - K, 0)
        start = zerolist[zstart]
        if start > 0:
            assert S[start - 1] == "1"
            start = oneindex[start - 1]
        val = end - start
        if maxval < val:
            maxval = val
            maxrange = (start, end)
    return maxval


print(solve())
