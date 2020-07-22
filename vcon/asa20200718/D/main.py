import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n = ini()
a = [ini() for _ in range(n)]
a.sort()


def solve():
    i = 0
    j = n - 1
    is_odd = n % 2 == 1
    a0 = a[i]
    a1 = a[j]
    s = a[j] - a[i]
    i += 1
    j -= 1
    while i < j:
        s += a[j] - a0
        s += a1 - a[i]
        a0 = a[i]
        a1 = a[j]
        i += 1
        j -= 1
    if is_odd and i == j:
        s += max(a1 - a[i], a[i] - a0)
    return s


print(solve())
