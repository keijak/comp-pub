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


def solve():
    n = ini()
    ans = 10 ** 7
    A = [None] * n
    B = [None] * n
    for i in range(n):
        a, b = inl()
        ans = min(ans, a + b)
        A[i] = a
        B[i] = b
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            ans = min(ans, max(A[i], B[j]))
    return ans


print(solve())
