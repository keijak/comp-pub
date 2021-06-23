import sys

n = int(sys.stdin.readline())
dic = {"a": 0, "b": 1, "c": 2}
S = [dic[c] for c in sys.stdin.readline().rstrip()]


def solve():
    ans = 0
    for i in range(n - 1, -1, -1):
        ans += S[i] + ans
    return ans


print(solve())
