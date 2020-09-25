import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()
M = 10 ** 9 + 7


def solve():
    if any([x == 0 for x in A]):
        return -1

    ans = 1
    for x in A:
        for k in range(2, x + 1):
            x = x ** k
            if x > M:
                return M
        ans *= x
        if ans > M:
            return M
    return M % ans


print(solve())
