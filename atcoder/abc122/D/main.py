import sys
import itertools

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
MOD = 10 ** 9 + 7
AGCT3 = ["".join(x) for x in itertools.product("AGCT", repeat=3)]
M = 64
blacklist3 = {"AGC", "ACG", "GAC"}


def solve():
    dp = [[0] * M for _ in range(N + 1)]
    for i in range(M):
        if AGCT3[i] not in blacklist3:
            dp[3][i] = 1
    for i in range(4, N + 1):
        for j in range(M):
            p3 = AGCT3[j]
            if p3 in blacklist3:
                dp[i][j] = 0
                continue
            for k in range(M):
                q3 = AGCT3[k]
                q2 = q3[1:]
                if not p3.startswith(q2):
                    continue
                if q3[0] == "A" and p3[1] == "G" and p3[2] == "C":
                    continue  # A?GC
                if q3[0] == "A" and p3[0] == "G" and p3[2] == "C":
                    continue  # AG?C
                dp[i][j] += dp[i - 1][k]
                dp[i][j] %= MOD
    ans = 0
    for x in dp[N]:
        ans += x
        ans %= MOD
    return ans


print(solve())
