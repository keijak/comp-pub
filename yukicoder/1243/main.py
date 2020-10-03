import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


def solve():
    a, b = inl()
    r = b - a
    ans = []
    while r:
        da = 1
        while a % (2 * da) == 0 and 2 * da <= r:
            da *= 2
        ans.append(da)
        r -= da
        a += da
    print(len(ans))
    print(*ans)


t = ini()
for i in range(t):
    solve()
