#!/usr/bin/env python3
import sys
import collections
import fractions

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()


MOD = 1000000007
N = ni()
cnt_ab0 = cnt_a0 = cnt_b0 = 0
cnt_r = collections.Counter()

for i in range(N):
    a, b = nm()
    if a == 0 and b == 0:
        cnt_ab0 += 1
    elif a == 0:
        cnt_a0 += 1
    elif b == 0:
        cnt_b0 += 1
    else:
        r = fractions.Fraction(a, b)
        cnt_r[r] += 1


def solve():
    rpairs = set()
    for r in cnt_r.keys():
        if r > 0:
            rpairs.add(r)
        else:
            rpairs.add(-1 / r)

    pow2 = [1] * (N + 1)
    for i in range(1, N + 1):
        pow2[i] = (pow2[i - 1] << 1) % MOD

    ans = 1
    for r1 in rpairs:
        r2 = -1 / r1
        p = pow2[cnt_r[r1]] + pow2[cnt_r[r2]] - 1
        ans *= p % MOD
        ans %= MOD
    p = pow(2, cnt_a0, MOD) + pow(2, cnt_b0, MOD) - 1
    ans *= p % MOD
    ans %= MOD
    ans += cnt_ab0 - 1
    return ans % MOD


if __name__ == "__main__":
    print(solve())
