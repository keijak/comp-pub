import sys
from collections import Counter

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, c = inl()
    a = [ini() for x in range(n)]

    b0, b1 = a[0::2], a[1::2]
    c0, c1 = Counter(b0), Counter(b1)

    m0 = c0.most_common(2)
    if len(m0) == 1:
        m0.append((-1, 0))
    m1 = c1.most_common(2)
    if len(m1) == 1:
        m1.append((-2, 0))

    if m0[0][0] != m1[0][0]:
        v0, v1 = m0[0][1], m1[0][1]
        return c * (len(b0) - v0 + len(b1) - v1)
    else:
        y0 = len(b0) - m0[0][1] + len(b1) - m1[1][1]
        y1 = len(b0) - m0[1][1] + len(b1) - m1[0][1]
        return c * min(y0, y1)


print(solve())
