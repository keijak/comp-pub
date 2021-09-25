import sys


def solve():
    inf = 10 ** 20

    def f(n):
        if n < 0:
            return inf
        if n == 0:
            return 0
        ans = inf
        b = 0
        while b <= 1:
            n2 = n // 10
            if n2 < b:
                break
            k = f(n2 - b)
            # print(f'{n2=} {b=} {k=}')
            d = n % 10 + b * 10
            lo = (d - 3 * k + 2) // 3
            hi = (d - k + 2) // 3 + 1
            if lo < hi and hi >= 2:
                ans = min(ans, k + max(lo - 1, 0))
                break
            b += 1
        # print(f'{n=} {ans=}', file=sys.stderr)
        return ans

    n = int(input())
    return f(n) + 1


t = int(input())
for i in range(t):
    print(solve())
