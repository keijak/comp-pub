import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


X, K, D = inl()


def solve():
    if X >= K * D:
        return X - K * D
    if X <= -K * D:
        return abs(X + K * D)
    y = X + K * D
    assert y > 0
    r = y // (2 * D)

    ans = y
    for k in range(r - 10, r + 10):
        if k < 0 or k > K:
            continue
        z = y - k * 2 * D
        ans = min(ans, abs(z))

    return ans


print(solve())
