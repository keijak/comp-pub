import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

MOD = 10 ** 9 + 7
N, M = inm()

t = [1]


def fact(x):
    if x >= len(t):
        for i in range(len(t), x + 1):
            t.append(t[-1] * i % MOD)
    return t[x]


def solve():
    k = 2
    m = M
    fs = []
    while k * k <= m:
        cnt = 0
        while m % k == 0:
            m //= k
            cnt += 1
        if cnt:
            fs.append(cnt)
        k += 1
    if m > 1:
        fs.append(1)

    ni = pow(fact(N - 1), MOD - 2, MOD)
    ans = 1
    for c in fs:
        ans = ans * fact(c + N - 1) % MOD
        ans = ans * ni % MOD
        ans = ans * pow(fact(c), MOD - 2, MOD) % MOD
    return ans


print(solve())
