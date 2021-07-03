MOD = 10 ** 9 + 7


def f(n):
    d, t, res = 1, 1, 0
    while t <= n:
        tt = t * 10
        if n < tt:
            s = (n - t + 1) * (t + n) // 2
        else:
            s = (tt - t) * (t + tt - 1) // 2
        res += d * s
        res %= MOD
        t = tt
        d += 1
    return res


L, R = map(int, input().split())
vr = f(R)
vl = f(L - 1)
ans = (vr - vl) % MOD
print(ans)
