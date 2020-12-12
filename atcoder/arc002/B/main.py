import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


mdays = {
    1: 31,
    2: 28,
    3: 31,
    4: 30,
    5: 31,
    6: 30,
    7: 31,
    8: 31,
    9: 30,
    10: 31,
    11: 30,
    12: 31,
}


def is_leap(y):
    if y % 400 == 0:
        return True
    if y % 100 == 0:
        return False
    return y % 4 == 0


def solve():
    y, m, d = map(int, ins().split("/"))
    while True:
        x = m * d
        if y % x == 0:
            print("{:04}/{:02}/{:02}".format(y, m, d))
            return
        if is_leap(y) and (m, d) == (2, 28):
            d += 1
        elif is_leap(y) and (m, d) == (2, 29):
            m, d = 3, 1
        elif (m, d) == (12, 31):
            y += 1
            m = d = 1
        elif d == mdays[m]:
            m += 1
            d = 1
        else:
            d += 1


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    solve()
