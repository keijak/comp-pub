import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = input()
    i = s.index("WWBWBWBWW")
    key = [
        "Mi",
        None,
        "Re",
        None,
        "Do",
        "Si",
        None,
        "La",
        None,
        "So",
        None,
        "Fa",
    ]
    return key[i]


print(solve())
