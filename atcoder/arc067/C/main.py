import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
MOD = 10 ** 9 + 7


def solve():
    fs = collections.Counter()
    for n in range(2, N + 1):
        i = 2
        while i * i <= n:
            while n % i == 0:
                n //= i
                fs[i] += 1
            i += 1
        if n > 1:
            fs[n] += 1

    ans = 1
    for c in fs.values():
        ans *= c + 1
        ans %= MOD

    return ans


print(solve())
