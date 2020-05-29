import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())

N = ni()
g1, s1, b1 = nm()
g2, s2, b2 = nm()


def solve():
    x1 = [g1, s1, b1]
    x2 = [g2, s2, b2]

    # dp[i] := max future donguri when we use i donguri
    dp = [0] * (N + 1)
    dp[0] = N
    for i in range(3):
        t1 = x1[i]
        t2 = x2[i]
        if t1 >= t2:
            continue
        for j in range(t1, N + 1):
            if dp[j - t1] >= N:
                dp[j] = max(dp[j], dp[j - t1] - t1 + t2)
    don = max(dp)
    dp = [0] * (don + 1)
    dp[0] = don
    for i in range(3):
        t1 = x1[i]
        t2 = x2[i]
        if t1 <= t2:
            continue
        for j in range(t2, don + 1):
            if dp[j - t2] >= don:
                dp[j] = max(dp[j], dp[j - t2] - t2 + t1)
    return max(dp)


print(solve())
