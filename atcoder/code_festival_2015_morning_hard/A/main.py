import sys
import itertools

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    a = inl()
    cum = list(itertools.accumulate(a, initial=0))

    rs = 0
    for i, x in enumerate(a):
        rs += i * x
    ls = 0
    rk = n - 1
    lk = 0

    ans = 10 ** 20
    for i in range(0, n, 2):
        res = rs + ls + rk * (rk - 1) // 2 + lk * (lk - 1) // 2
        ans = min(ans, res)
        # debug(f"{i=} {res=} {ls=} {lk=} {rs=} {rk=}")
        rs -= 2 * (cum[n] - (cum[i + 2] if i + 2 <= n else 0))
        rs -= a[i + 1] if i + 1 < n else 0
        ls += 2 * cum[i + 1]
        ls += a[i + 1] if i + 1 < n else 0
        rk -= 2
        lk += 2

    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
