import sys
import math

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    a = [x - 1 for x in inl()]
    a.sort()
    a.append(n)
    if m >= n:
        return 0
    if m == 0:
        return 1
    p = -1
    g = None
    for i in range(m + 1):
        d = a[i] - p
        if d > 1:
            if g is None:
                g = d - 1
            else:
                g = min(g, d - 1)
        p = a[i]
    assert g is not None
    # debug(f"{g=}")

    ans = 0
    p = -1
    for i in range(m + 1):
        d = a[i] - p
        if d > 1:
            x = d - 1
            ans += (x + g - 1) // g
        p = a[i]

    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
