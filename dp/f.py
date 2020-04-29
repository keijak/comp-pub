import bisect
import collections
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

S = readline().rstrip()
T = readline().rstrip()
if len(S) < len(T):
    S, T = T, S


def to_str(ss):
    buf = []
    while ss is not None:
        s, ss = ss
        buf.append(s)
    buf.reverse()
    return "".join(buf)


def solve():
    index = collections.defaultdict(list)
    for i, sc in enumerate(S):
        index[sc].append(i)
    dp = [None] * len(T)
    for i, tc in enumerate(T):
        if tc not in index:
            continue
        dn = list(dp)
        ii = index[tc]
        if dn[0] is None or dn[0][0] > ii[0]:
            dn[0] = (ii[0], [chr(tc), None])
        for j in range(1, len(T)):
            if dp[j - 1] is None:
                break
            k = bisect.bisect_right(ii, dp[j - 1][0])
            if k < len(ii) and (dn[j] is None or dn[j][0] > ii[k]):
                dn[j] = (ii[k], [chr(tc), dp[j - 1][1]])
        dp = dn
    for i in range(len(T) - 1, -1, -1):
        if dp[i] is not None:
            return to_str(dp[i][1])
    return ""


if __name__ == "__main__":
    print(solve())
