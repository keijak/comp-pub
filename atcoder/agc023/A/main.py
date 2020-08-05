import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()


def solve():
    cum = [0] * (N + 1)
    for i in range(N - 1, -1, -1):
        cum[i] = cum[i + 1] + A[i]
    count = Counter()
    ans = 0
    for i in range(N):
        if A[i] == 0:
            ans += 1
        ans += count[cum[i] - A[i]]
        count[cum[i]] += 1
    return ans


print(solve())
