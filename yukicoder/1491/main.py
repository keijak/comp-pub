import sys


def solve():
    k = int(sys.stdin.readline())
    ans = k * (k + 1) // 2
    ans += (k + 1) * (k + 2) * 2
    ans -= (k + 1) * 4
    ans += 1

    if k >= 2:
        ans += k * (k - 1)
    if k >= 3:
        ans += (k - 1) * (k - 2) // 2
    return ans


print(solve())
