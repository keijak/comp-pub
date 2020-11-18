import sys
from collections import Counter

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, c = inl()
    a = [ini() for x in range(n)]

    ans = c * n
    for x in range(1, 11):
        for y in range(1, 11):
            if x == y:
                continue
            cost = 0
            for i in range(n):
                if i % 2 == 0:
                    if a[i] != x:
                        cost += c
                else:
                    if a[i] != y:
                        cost += c
            ans = min(ans, cost)

    return ans


print(solve())
