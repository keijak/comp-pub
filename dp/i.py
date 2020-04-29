import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
P = list(map(float, readline().split()))


def solve():
    dp = [0.0] * (N + 1)
    dp[0] = 1.0 - P[0]
    dp[1] = P[0]
    for i in range(1, N):
        ndp = [0.0] * (N + 1)
        p = P[i]
        for j in range(i, -1, -1):
            ndp[j + 1] += dp[j] * p
            ndp[j] += dp[j] * (1 - p)
        dp = ndp
    return sum(dp[N // 2 + 1 :])


if __name__ == "__main__":
    print(solve())
