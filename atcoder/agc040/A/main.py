import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def runlength(s):
    res = []
    if s[0] == ">":
        res.append(["<", 0])
    c = [s[0], 1]
    for x in s[1:]:
        if x == c[0]:
            c[1] += 1
        else:
            res.append(c)
            c = [x, 1]
    res.append(c)
    if c[0] == "<":
        res.append([">", 0])
    assert len(res) % 2 == 0
    return res


def solve():
    s = ins()
    r = runlength(s)
    m = len(r)
    ans = 0
    for i in range(0, m, 2):
        o1, c1 = r[i]
        o2, c2 = r[i + 1]
        assert o1 == "<", i
        assert o2 == ">", i
        x = max(c1, c2)
        y = min(c1, c2)
        ans += x * (x + 1) // 2
        ans += y * (y - 1) // 2

    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
