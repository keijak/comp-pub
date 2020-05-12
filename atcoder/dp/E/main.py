import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, W = map(int, readline().split())
things = []
vsum = 0
for i in range(N):
    weight, value = map(int, readline().split())
    things.append((weight, value))
    vsum += value


INF = 1 << 62


def solve():
    dp = [INF] * (vsum + 1)
    w0, v0 = things[0]
    dp[0] = 0
    dp[v0] = w0
    for i in range(1, N):
        dn = list(dp)
        w, v = things[i]
        for j in range(vsum - v + 1):
            w2 = dp[j] + w
            if j + v <= vsum and w2 <= W:
                dn[j + v] = min(dn[j + v], w2)
        dp = dn
    for i in range(vsum, -1, -1):
        w = dp[i]
        if w <= W:
            return i
    return 0  # nothing is available


if __name__ == "__main__":
    print(solve())
