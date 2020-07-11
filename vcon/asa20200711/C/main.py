import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()


def solve():
    k = 26
    a = 0
    l = 1
    while a + k < N:
        a += k
        k *= 26
        l += 1
    m = N - a - 1
    s = [None] * l
    for i in range(l):
        d = m % 26
        s[i] = chr(ord("a") + d)
        m //= 26
    return "".join(reversed(s))


print(solve())
