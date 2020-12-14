import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


MOD = 1000000007


def solve():
    n = ini()
    a = [ini() for _ in range(n)]
    C = 6000
    M = n + 10
    dp = [[0] * C for _ in range(2)]
    dp[0][0] = 1
    for i in range(M - 1):
        b = a[i] if i < n else 0
        for j in range(C):
            budget = b + j
            for d in range(10):
                if d > budget:
                    break
                nj = (budget - d) // 10
                dp[1][nj] += dp[0][j]
                dp[1][nj] %= MOD

        dp[0], dp[1] = dp[1], dp[0]
        for j in range(C):
            dp[1][j] = 0

    return (dp[0][0] - 1) % MOD


if __name__ == "__main__":
    # try:
    #     import frosch

    #     frosch.hook()
    # except ImportError:
    #     pass
    print(solve())
