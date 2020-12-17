import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def kw_match(w, ng):
    if len(w) != len(ng):
        return False
    for i, x in enumerate(ng):
        if x == "*":
            continue
        if w[i] != x:
            return False
    return True


def solve():
    s = ins().split(" ")
    n = ini()
    t = [ins() for _ in range(n)]
    for i in range(len(s)):
        w = s[i]
        for ng in t:
            if kw_match(w, ng):
                s[i] = "*" * len(w)
                break
    return " ".join(s)


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(solve())
