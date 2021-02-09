import fractions

# a[n+1] + 1/k = (k+1)(a[n] + 1/k)

T = 2 * 10 ** 12
a, k = map(int, input().split())
if k == 0:
    print(T - a)
else:
    rk = fractions.Fraction(1, k)
    fv, tv = (0, 45)
    while tv - fv > 1:
        mid = (tv + fv) // 2
        if ((k + 1) ** mid) * (a + rk) >= T + rk:
            tv = mid
        else:
            fv = mid
    print(tv)
