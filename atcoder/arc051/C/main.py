import sys
from collections import deque

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


MOD = 10 ** 9 + 7


def solve():
    N, A, B = inl()
    a = inl()
    a.sort()

    if N == 1:
        x = a[0] * pow(A, B, MOD)
        print(x % MOD)
        exit(0)

    if A == 1:
        for x in a:
            print(x % MOD)
        exit(0)

    while True:
        a[0] *= A
        B -= 1
        if B == 0:
            break
        if a[0] >= a[-1]:
            break
        a.sort()
    a.sort()

    p = B // N
    r = B % N
    for i in range(N):
        a[i] %= MOD
        a[i] *= pow(A, p, MOD)
        a[i] %= MOD
    a = deque(a)
    for i in range(r):
        a[0] *= A
        a[0] %= MOD
        x = a.popleft()
        a.append(x)
    for x in a:
        print(x)


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    solve()
