import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, readline().split())
A = list(map(int, input().split()))
A.sort()
amin, amax = A[0], A[-1]


def solve():
    dp = [None] * (K + 1)
    for i in range(amin):
        dp[i] = False
    for i in range(amin, K + 1):
        winnable = False
        for a in A:
            j = i - a
            if j < 0:
                break
            assert dp[j] is not None
            if not dp[j]:
                dp[i] = True
                winnable = True
                break
        if not winnable:
            dp[i] = False
    return dp[K]


if __name__ == "__main__":
    print("First" if solve() else "Second")
