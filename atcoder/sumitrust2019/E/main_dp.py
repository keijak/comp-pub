import itertools
import sys
from collections import Counter

sys.setrecursionlimit(10 ** 9)
input = sys.stdin.readline
M = 1000000007
N = int(input())
left = list(map(int, input().split()))
counter = Counter(left)


def dp(t, n1, n2, n3):
    cached = t.get((n1, n2, n3))
    if cached is not None:
        return cached
    remaining = n1 + n2 + n3
    assert remaining > 0
    last_cnt = left[remaining - 1] + 1
    res = 0
    if last_cnt == n1:
        res += dp(t, *sorted([n1 - 1, n2, n3]))
        res %= M
    if last_cnt == n2:
        res += dp(t, *sorted([n1, n2 - 1, n3]))
        res %= M
    if last_cnt == n3:
        res += dp(t, *sorted([n1, n2, n3 - 1]))
        res %= M
    # print(f"{remaining}: ({n1},{n2},{n3}) => {res}")
    t[n1, n2, n3] = res
    return res


def solve():
    h = [0, 0, 0]
    for i in range(N):
        k = counter[i]
        if k == 3:
            h[2] = h[1] = h[0] = i + 1
        elif k == 2:
            h[2] = h[1] = i + 1
        elif k == 1:
            h[2] = i + 1
        else:
            break
    if sum(h) != N:
        return 0
    t = dict()
    t[0, 0, 0] = 1
    res = dp(t, *h)
    return (res * len(set(itertools.permutations(h)))) % M


print(solve())
