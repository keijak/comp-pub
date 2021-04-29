import sys


def solve():
    n = int(sys.stdin.readline())
    ss = set()
    ans = []
    for i in range(n):
        s = sys.stdin.readline().rstrip()
        if s not in ss:
            ss.add(s)
            ans.append(i + 1)
    return ans


print(*solve(), sep="\n")
