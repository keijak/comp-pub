N, *ng = map(int, open(0).read().split())
ng.sort()
INF = 10000


def solve():
    if N in ng:
        return False
    minsteps = [INF] * (N + 1)
    minsteps[N] = 0
    for i in range(N - 1, -1, -1):
        if i in ng:
            continue
        m = INF
        if i + 1 <= N:
            m = min(m, minsteps[i + 1] + 1)
        if i + 2 <= N:
            m = min(m, minsteps[i + 2] + 1)
        if i + 3 <= N:
            m = min(m, minsteps[i + 3] + 1)
        minsteps[i] = m
    return minsteps[0] <= 100


print("YES" if solve() else "NO")
