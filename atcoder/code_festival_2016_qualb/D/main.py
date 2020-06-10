import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
A = [ini() for _ in range(N)]


def solve():
    p_min = 1
    ans = 0
    # debug(A)
    for i in range(N):
        if A[i] < p_min:
            continue
        if A[i] == p_min:
            p_min += 1
            continue
        k = (A[i] - 1) // p_min
        ans += k
        A[i] = 1
        if p_min == 1:
            p_min += 1
        # debug(f">> {i=} {p_min=} {k=} {ans=} {A=}")
    return ans


print(solve())
