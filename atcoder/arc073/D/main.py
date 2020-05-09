import sys
import collections

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, W = map(int, readline().split())
items = []
for i in range(N):
    w, v = map(int, readline().split())
    items.append((w, v))


def solve():
    dp = collections.defaultdict(lambda: -1)
    dp[0] = 0
    for i in range(N):
        wi, vi = items[i]
        ndp = collections.defaultdict(lambda: -1)
        for ws, vs in dp.items():
            if ws + wi <= W:
                ndp[ws + wi] = max(ndp[ws + wi], vs + vi)
        for wt, vt in ndp.items():
            dp[wt] = max(dp[wt], vt)
    return max(dp.values())


if __name__ == "__main__":
    print(solve())
