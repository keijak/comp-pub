import sys
from collections import Counter
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    h, w = inl()
    counter = Counter()
    for i in range(h):
        for c in ins():
            counter[c] += 1

    p4 = (h // 2) * (w // 2)
    p1 = 1 if h % 2 == 1 and w % 2 == 1 else 0
    p2 = (h * w - p4 * 4 - p1) // 2

    xs = [-x for x in counter.values()]
    hp = []
    for x in xs:
        heapq.heappush(hp, x)

    while hp:
        x = heapq.heappop(hp)
        k = -x
        if p4 > 0:
            if k < 4:
                return False
            m = k // 4
            p = min(m, p4)
            p4 -= p
            k -= p * 4
            if k > 0:
                heapq.heappush(hp, -k)
        elif p2 > 0:
            if k < 2:
                return False
            m = k // 2
            p = min(m, p2)
            p2 -= p
            k -= p * 2
            if k > 0:
                heapq.heappush(hp, -k)
        elif p1 > 0:
            if k == 1:
                return True
            return False
    return True


print("Yes" if solve() else "No")
