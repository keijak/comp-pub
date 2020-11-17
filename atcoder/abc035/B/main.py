import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

d = {
    "L": (-1, 0),
    "R": (1, 0),
    "U": (0, 1),
    "D": (0, -1),
}


def solve():
    s = ins()
    cx, cy = 0, 0
    cq = 0
    for c in s:
        if c == "?":
            cq += 1
        else:
            dx, dy = d[c]
            cx += dx
            cy += dy
    m = abs(cx) + abs(cy)
    t = ini()
    if t == 1:
        return m + cq
    elif cq <= m:
        return m - cq
    else:
        x = cq - m
        return x % 2


print(solve())
