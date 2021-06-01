import sys


def solve(n, d):
    if n % 2 == 1 or d == 0 or 2 * d > n:
        return 0
    if 2 * d == n:
        return 1

    dp = [[-1 for j in range(d + 1)] for i in range(n + 1)]

    def f(m, k):
        if m == 0:
            return 1
        if k == 0:
            return 0
        if dp[m][k] >= 0:
            return dp[m][k]
        res = 0
        for i in range(1, m, 2):
            l = i - 1
            r = m - 1 - i
            lv = f(l, k - 1)
            if lv > 0:
                rv = f(r, k)
                res += lv * rv
        dp[m][k] = res
        return res

    return f(n, d) - f(n, d - 1)


try:
    while True:
        inp = list(map(int, input().split()))
        if len(inp) != 2:
            continue
        n, d = inp
        print(solve(n, d))
except:
    pass
