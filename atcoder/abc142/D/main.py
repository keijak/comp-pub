import sys
import fractions

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

A, B = inm()


def solve():
    x = fractions.gcd(A, B)
    ans = 0
    for k in range(2, x + 1):
        if k * k > x:
            break
        cnt = 0
        while x % k == 0:
            x //= k
            cnt += 1
        if cnt:
            ans += 1
    if x > 1:
        ans += 1
    return ans + 1


print(solve())
