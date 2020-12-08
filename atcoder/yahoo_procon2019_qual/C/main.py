import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    k, a, b = inl()
    x = b - a
    ans = k + 1
    if x <= 2:
        return ans
    cur = 1
    if k >= a + 1:
        k -= a + 1
        cur += a - 1
        cur += x
        # debug(f"{cur=} {x=} {k=}")

        cur += (k // 2) * x
        # debug(f"{cur=}")
        if k & 1:
            cur += 1
        # debug(f"{cur=}")
        ans = max(ans, cur)
    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())