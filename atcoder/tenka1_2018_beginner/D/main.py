import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def decomp(n):
    m = 1
    while True:
        p = m * (m + 1)
        if p == 2 * n:
            return m
        if p > 2 * n:
            return None
        m += 1


def solve():
    n = ini()
    m = decomp(n)
    if m is None:
        print("No")
        return
    S = [[] for _ in range(m + 1)]

    counter = 1
    for i in range(m + 1):
        for j in range(i):
            S[i].append(counter)
            S[j].append(counter)
            counter += 1

    print("Yes")
    print(m + 1)  # = k
    for i in range(m + 1):
        assert len(S[i]) == m
        print(m, end=" ")
        print(*S[i])


solve()
