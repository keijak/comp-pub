import sys
from collections import Counter

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def binom(n, k):
    x = 1
    for i in range(k):
        x *= n - i
    for i in range(k):
        x //= i + 1
    return x


def solve():
    s = sys.stdin.readline().rstrip()
    c = Counter(s)
    co = c["o"]
    if co > 4:
        return 0
    if co == 4:
        return 4 * 3 * 2 * 1
    cq = c["?"]
    if co == 3:
        return 12 * 3 + 4 * 3 * 2 * cq
    if co == 2:
        return 8 + 12 + cq * 4 * 3 + binom(cq, 2) * 4 * 3 * 2
    if co == 1:
        ans = 1 + cq * 4
        ans += binom(cq, 2)
    return ans


print(solve())
