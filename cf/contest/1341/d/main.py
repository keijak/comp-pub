import sys

sys.setrecursionlimit(10 ** 9)
input = sys.stdin.readline

n, k = map(int, input().split())
bins = []
for _ in range(n):
    bins.append(input().rstrip())

segs = [
    "1110111",
    "0010010",
    "1011101",
    "1011011",
    "0111010",
    "1101011",
    "1101111",
    "1010010",
    "1111111",
    "1111011",
]

INF = 5000


def diff(s1, s2):
    cnt = 0
    for i in range(7):
        if s1[i] == s2[i]:
            continue
        if s1[i] == "1" and s2[i] == "0":
            cnt += 1
        else:
            return INF
    return cnt


maxdiff = [INF] * n
mindiff = [0] * n
for i in range(n):
    for j in range(10):
        d = diff(segs[j], bins[i])
        mindiff[i] = min(mindiff[i], d)
        maxdiff[i] = max(maxdiff[i], d)
acummin = [0] * n
acummax = [0] * n
for i in range(n - 2, -1, -1):
    acummin[i] = acummin[i + 1] + mindiff[i + 1]
    acummax[i] = acummax[i + 1] + maxdiff[i + 1]


def finish(ans):
    out = []
    while ans:
        d, ans = ans
        out.append(d)
    out.reverse()
    print(*out, sep="")


dp = [[True] * (k + 1) for _ in range(n + 1)]
ans = [0] * n


def solve(i, sticks):
    if not dp[i][sticks]:
        return False
    if i == n:
        if sticks == 0:
            print(*ans, sep="")
            return True
        return False
    digit = bins[i]
    amin, amax = acummin[i], acummax[i]
    for j in range(9, -1, -1):
        sd = diff(segs[j], digit)
        rem = sticks - sd
        if amin <= rem <= amax:
            ans[i] = j
            if solve(i + 1, rem):
                return True
    dp[i][sticks] = False
    return False


if not solve(0, k):
    print(-1)
