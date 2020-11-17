import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    pw = ins()
    if pw == "a" or pw == ("z" * 20):
        return "NO"
    if pw == "a" * len(pw):
        return "b" + ("a" * (len(pw) - 2))
    ps = list(pw)
    dec = inc = None
    for i in range(len(pw)):
        if ps[i] != "a" and (dec is None or dec == inc):
            dec = i
        if ps[i] != "z" and (inc is None or dec == inc):
            inc = i

    assert dec is not None
    ps[dec] = chr(ord(ps[dec]) - 1)
    if len(pw) < 20:
        ps.append("a")
    else:
        assert inc is not None
        ps[inc] = chr(ord(ps[inc]) + 1)
    return "".join(ps)


print(solve())
