import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, W = map(int, readline().split())
things = []
for i in range(N):
    weight, value = map(int, readline().split())
    things.append((weight, value))


def solve():
    dp = [0] * (W + 1)
    w0, v0 = things[0]
    for i in range(w0, W + 1):
        dp[i] = v0
    for i in range(1, N):
        dn = list(dp)
        weight, value = things[i]
        for j in range(weight, W + 1):
            dn[j] = max(dn[j], dp[j - weight] + value)
        dp = dn
    return dp[W]


if __name__ == "__main__":
    print(solve())
