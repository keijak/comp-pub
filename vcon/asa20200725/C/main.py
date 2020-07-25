import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

A, B = inm()


def leap(x):
    if x % 400 == 0:
        return True
    if x % 100 == 0:
        return False
    return x % 4 == 0


def solve():
    x = (A + 399) // 400
    y = B // 400
    ans = 0
    if 400 * x <= B and A <= 400 * y and x < y:
        ans += (y - x) * 97
    m = min(400 * x, B + 1)
    for i in range(A, m):
        if leap(i):
            ans += 1
    for i in range(max(400 * y, m), B + 1):
        if leap(i):
            ans += 1

    return ans


print(solve())
