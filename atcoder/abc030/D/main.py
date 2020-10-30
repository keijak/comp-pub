import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, a = inl()
    a -= 1
    k = ini()
    b = [x - 1 for x in inl()]

    def sim(cur, r):
        while r:
            cur = b[cur]
            r -= 1
        return cur

    if k < n:
        return sim(a, k)

    i = 0
    c = a
    seen = {a: 0}
    while True:
        d = b[c]
        if d in seen:
            j = seen[d]
            r = (k - (i + 1)) % (i - j)
            return sim(d, r)
        else:
            seen[d] = i
            i += 1
        c = d
    return c


print(solve() + 1)
