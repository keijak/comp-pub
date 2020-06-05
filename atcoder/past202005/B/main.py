import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *args, **kwargs: print(*args, **kwargs, file=sys.stderr)

N, M, Q = inm()


def solve():
    sol = [N] * M
    pt = [set() for _ in range(N)]
    for i in range(Q):
        c, n, *ms = inm()
        n -= 1
        if c == 1:
            ans = 0
            for p in pt[n]:
                ans += sol[p]
            print(ans)
        else:
            assert c == 2
            assert len(ms) == 1
            m = ms[0] - 1
            pt[n].add(m)
            sol[m] -= 1
            # debug(f"{n} solved {m} got {sol[m]} point {pt[n]}")


solve()
