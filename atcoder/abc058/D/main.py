import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

MOD = 10 ** 9 + 7
N, M = inm()
X = inl()
Y = inl()
assert len(X) == N
assert len(Y) == M


def solve():
    xsum = [0] * N
    xsum[N - 1] = (X[N - 1] - X[N - 2]) % MOD
    for k in range(2, N):
        xsum[N - k] = xsum[N - k + 1] + k * (X[N - k] - X[N - k - 1])
        xsum[N - k] %= MOD

    ysum = [0] * M
    ysum[M - 1] = (Y[M - 1] - Y[M - 2]) % MOD
    for k in range(2, M):
        ysum[M - k] = ysum[M - k + 1] + k * (Y[M - k] - Y[M - k - 1])
        ysum[M - k] %= MOD

    return sum(xsum) % MOD * (sum(ysum) % MOD) % MOD


print(solve())
