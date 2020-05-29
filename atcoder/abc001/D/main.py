import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
times = []
for i in range(N):
    s, e = map(int, ns().split("-"))
    times.append((s, e))


def dectime(t):
    z = t * 5
    m = z % 60
    h = z // 60
    return "{:02}{:02}".format(h, m)


def solve():
    tt = [False] * (24 * 12 + 1)
    for s, e in times:
        sh = s // 100
        sm = s % 100
        sx = sh * 12 + sm // 5
        eh = e // 100
        em = e % 100
        ex = eh * 12 + (em + 4) // 5
        for x in range(sx, ex):
            tt[x] = True

    start = None
    for i, rain in enumerate(tt):
        if rain and start is None:
            start = i
        elif not rain and start is not None:
            print("{}-{}".format(dectime(start), dectime(i)))
            start = None


solve()
