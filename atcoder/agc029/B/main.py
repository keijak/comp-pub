import sys
import bisect
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()
A.sort()


def solve():
    ac = collections.Counter(A)
    num_pairs = 0
    for i in range(N - 1, 0, -1):
        x = A[i]
        if ac[x] <= 0:
            continue
        p = 1
        while p <= x:
            p <<= 1
        j = bisect.bisect_left(A, p - x, 0, i)
        if j >= i:
            continue
        y = A[j]
        if x + y != p:
            continue
        ac[x] -= 1
        if ac[y] <= 0:
            continue
        ac[y] -= 1
        num_pairs += 1
    return num_pairs


print(solve())
