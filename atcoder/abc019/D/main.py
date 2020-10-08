import sys

debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = int(input())
    md = -1
    mi = None
    for i in range(1, n):
        d = int(input(f"? {n} {i}\n"))
        debug(f"{d=}")
        if md < d:
            md = d
            mi = i
    md = -1
    for i in range(1, n + 1):
        if i == mi:
            continue
        d = int(input(f"? {mi} {i}\n"))
        debug(f"{d=}")
        if md < d:
            md = d
    print(f"! {md}")


solve()
