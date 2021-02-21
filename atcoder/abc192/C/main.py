def f(x):
    g1 = int("".join(sorted(str(x), reverse=True)))
    g2 = int("".join(sorted(str(x))))
    return g1 - g2


def solve():
    n, k = map(int, input().split())
    x = n
    for i in range(k):
        x = f(x)
    return x


print(solve())
