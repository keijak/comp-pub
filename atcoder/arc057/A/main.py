import fractions

T = 2 * 10 ** 12


def solve():
    a, k = map(int, input().split())
    if k == 0:
        return T - a
    rk = fractions.Fraction(1, k)
    for i in range(45):
        if ((k + 1) ** i) * (a + rk) >= T + rk:
            return i


print(solve())