import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    v = inl()
    ev = collections.Counter()
    od = collections.Counter()
    for i in range(n):
        if i % 2 == 0:
            ev[v[i]] += 1
        else:
            od[v[i]] += 1
    ex = ev.most_common(2)
    ox = od.most_common(2)
    if ex[0][0] != ox[0][0]:
        ans = n - ex[0][1] - ox[0][1]
    elif len(ex) == 1 and len(ox) == 1:
        ans = n - ex[0][1]
    elif len(ex) == 1:
        ans = n - ex[0][1] - ox[1][1]
    elif len(ox) == 1:
        ans = n - ex[1][1] - ox[0][1]
    else:
        ans = min(n - ex[0][1] - ox[1][1], n - ex[1][1] - ox[0][1])
    return ans


print(solve())
