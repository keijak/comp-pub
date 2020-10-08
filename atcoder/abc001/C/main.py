import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def to_dir(deg, wind):
    if wind == 0:
        return "C"
    ds = [
        "N",
        "NNE",
        "NE",
        "ENE",
        "E",
        "ESE",
        "SE",
        "SSE",
        "S",
        "SSW",
        "SW",
        "WSW",
        "W",
        "WNW",
        "NW",
        "NNW",
    ]
    deg *= 10
    u = 1125
    for d in ds:
        if deg < u:
            return d
        u += 2250
    return "N"


def to_wind(dis):
    wps = int(dis / 6.0 + 0.5)
    ws = [2, 15, 33, 54, 79, 107, 138, 171, 207, 244, 284, 326]
    for i, w in enumerate(ws):
        if wps <= w:
            return i
    return 12


def solve():
    deg, dis = inl()
    w = to_wind(dis)
    return to_dir(deg, w), w


print(*solve())
