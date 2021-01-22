import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

MOD = 10 ** 9 + 7


def fact(n):
    r = 1
    for i in range(1, n + 1):
        r *= i
        r %= MOD
    return r


def solve():
    n = ini()
    s = ins()
    n2 = n * 2
    endp = [None] * n2
    for i in range(n2):
        if i % 2 == 0:
            endp[i] = s[i] == "W"
        else:
            endp[i] = s[i] == "B"

    ce = 0
    for x in endp:
        if x:
            ce += 1
    if ce != n:
        return 0

    ans = 1
    ns = 0
    for i in range(n2):
        if endp[i]:
            if ns == 0:
                return 0
            ans *= ns
            ans %= MOD
            ns -= 1
        else:
            ns += 1
    if ns != 0:
        return 0
    ans *= fact(n)
    ans %= MOD
    return ans


print(solve())
