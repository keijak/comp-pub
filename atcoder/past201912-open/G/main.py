import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N = ini()
A = [[0] * N for _ in range(N)]
for i in range(N - 1):
    a = inl()
    assert len(a) == N - i - 1
    for j, x in enumerate(a):
        A[i][1 + i + j] = x

INF = 1 << 60


def asum(a):
    s = 0
    for i in range(len(a)):
        for j in range(i):
            s += A[a[j]][a[i]]
    return s


def solve(i, a, b, c):
    if i == N:
        return asum(a) + asum(b) + asum(c)

    mx = -INF
    a.append(i)
    mx = max(mx, solve(i + 1, a, b, c))
    a.pop()

    b.append(i)
    mx = max(mx, solve(i + 1, a, b, c))
    b.pop()

    c.append(i)
    mx = max(mx, solve(i + 1, a, b, c))
    c.pop()
    return mx


def main():
    print(solve(1, [0], [], []))


main()
