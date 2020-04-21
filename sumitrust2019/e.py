import itertools
import sys
import numpy as np
from collections import Counter
import bisect

sys.setrecursionlimit(10 ** 9)
input = sys.stdin.readline
M = 1000000007
N = int(input())
left = list(map(int, input().split()))

indices = Counter()
for i, x in enumerate(left):
    indices[x] += 1


def dp(t, ns):
    cached = t.get(ns)
    if cached is not None:
        return cached
    remaining = sum(ns)
    assert remaining > 0
    last_cnt = left[remaining - 1] + 1
    n1, n2, n3 = ns
    res = 0
    if last_cnt == n1:
        res += dp(t, tuple(sorted([n1 - 1, n2, n3])))
        res %= M
    if last_cnt == n2:
        res += dp(t, tuple(sorted([n1, n2 - 1, n3])))
        res %= M
    if last_cnt == n3:
        res += dp(t, tuple(sorted([n1, n2, n3 - 1])))
        res %= M
    # print(f"{remaining}: ({n1},{n2},{n3}) => {res}")
    t[ns] = res
    return res


def solve():
    h = [0, 0, 0]
    for i in range(N):
        k = indices[i]
        if k == 3:
            h[0] = h[1] = h[2] = i + 1
        elif k == 2:
            h[0] = h[1] = i + 1
        elif k == 1:
            h[0] = i + 1
        else:
            break
    assert sum(h) == N
    t = dict()
    t[0, 0, 0] = 1
    res = dp(t, tuple(sorted(h))
    return res * len(set(itertools.permutations(h))) % M


print(solve())
