P = float(input())


def f(t):
    # c*P = c * 2.0 ** (t/1.5) * Q
    # Q = P / (2.0 ** (t/1.5))
    return t + (P / (2.0 ** (t / 1.5)))


def ternary_search(f, xmin, xmax):
    l, r = xmin, xmax
    while r - l > 1e-6:
        d = (r - l) / 3.0
        m1 = l + d
        m2 = l + 2.0 * d
        try:
            f1 = f(m1)
            f2 = f(m2)
        except OverflowError:
            r = m2
        else:
            if f1 < f2:
                r = m2
            else:
                l = m1
    return l


print(f(ternary_search(f, 0.0, P)))
