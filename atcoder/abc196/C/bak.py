import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    S = ins()
    m = len(S)
    ans = 0
    # debug(f"{m=} {m//2}")
    for d in range(1, m // 2 + 1):
        # debug(f"{d=}")
        if d * 2 >= m:
            break
        ans += 9 * 10 ** (d - 1)
    if m % 2 == 1:
        return ans

    for i in range(m // 2 + 1):
        if i == m // 2:
            ans += 1
            break
        x = ord(S[i]) - ord("0")
        y = ord(S[m - 1 - i]) - ord("0")
        # debug(f"{i=} {x=} {y=}")
        d = m // 2 - i
        z = x - 1 if i == 0 else x
        ans += z * 10 ** (d - 1)
        if y < x:
            break

    return ans


print(solve())
