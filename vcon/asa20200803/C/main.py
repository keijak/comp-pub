import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


N = ini()
A = inl()


def solve():
    asum = sum(A)
    if asum % N != 0:
        return -1
    b = asum // N

    count = 0
    diff = 0
    for i in range(N):
        if A[i] != b + diff:
            count += 1
        diff += b - A[i]

    return count


print(solve())
