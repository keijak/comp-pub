import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print(*a, **dict(file=sys.stderr, **kw))

N = ini()
A = [ini() for _ in range(N)]


def solve():
    c = 0
    ans = 0
    for i in range(N - 1, -1, -1):
        s = A[i]
        if s > i:
            return -1
        if s < c:
            return -1
        if s > c:
            ans += s
        c = s - 1
    return ans


print(solve())
