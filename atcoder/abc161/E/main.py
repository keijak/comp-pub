#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, K, C = nm()
S = ns()
assert len(S) == N

INF = 1 << 60


def solve():
    lfilled = [0] * N  # max left count where i-th position is filled
    lempty = [0] * N  # max left count where i-th position is empty
    for i in range(0, N):
        if i > 0:
            lempty[i] = max(lempty[i - 1], lfilled[i - 1])
        if S[i] == "x":
            lfilled[i] = -INF
        elif i - C >= 0:
            lfilled[i] = lempty[i - C] + 1
        else:
            lfilled[i] = 1

    rfilled = [0] * N
    rempty = [0] * N
    for i in range(N - 1, -1, -1):
        if i < N - 1:
            rempty[i] = max(rempty[i + 1], rfilled[i + 1])
        if S[i] == "x":
            rfilled[i] = -INF
        elif i + C < N:
            rfilled[i] = rempty[i + C] + 1
        else:
            rfilled[i] = 1

    def must_work(s, t):
        l = max(lfilled[s - 1], lempty[s - 1]) if s - 1 >= 0 else 0
        r = max(rfilled[t], rempty[t]) if t < N else 0
        # print(f"day {s}-{t}: l+r={l+r} l={l} r={r}", file=sys.stderr)
        return l + r < K

    # imos
    no_need_to_work = [0] * (N + C)
    for t in range(1, N + C):
        s = max(t - C, 0)
        if not must_work(s, t):
            no_need_to_work[s] += 1
            no_need_to_work[t] -= 1
    acc = [0] * (N + C + 1)
    ans = []
    for i in range(1, N + 1):
        acc[i] += acc[i - 1] + no_need_to_work[i - 1]
        if acc[i] == 0:
            ans.append(i)
    return ans


def solve0():
    workable = [i + 1 for i, x in enumerate(S) if x == "o"]
    if len(workable) == K:
        return workable
    assert len(workable) > K
    return []


if C == 0:
    print(*solve0(), sep="\n")
else:
    print(*solve(), sep="\n")
