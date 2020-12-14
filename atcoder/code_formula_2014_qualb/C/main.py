import sys
import itertools

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    a = ins()
    b = ins()
    n = len(a)

    nop = False
    charset = set()
    diffs = []
    for i in range(n):
        if a[i] == b[i]:
            if a[i] in charset:
                nop = True
            charset.add(a[i])
        else:
            diffs.append(i)

    ndiff = len(diffs)
    if ndiff > 6:
        return False
    if ndiff == 0:
        return nop
    # debug(f"{ndiff=}")

    da, db = [], []
    for i in diffs:
        da.append(a[i])
        db.append(b[i])

    if sorted(da) != sorted(db):
        return False

    for c1, c2 in itertools.combinations(range(ndiff), 2):
        da[c1], da[c2] = da[c2], da[c1]
        if da == db:
            return True
        for c3, c4 in itertools.combinations(range(ndiff), 2):
            da[c3], da[c4] = da[c4], da[c3]
            if nop and da == db:
                return True
            for c5, c6 in itertools.combinations(range(ndiff), 2):
                da[c5], da[c6] = da[c6], da[c5]
                if da == db:
                    return True
                da[c5], da[c6] = da[c6], da[c5]
            da[c3], da[c4] = da[c4], da[c3]
        da[c1], da[c2] = da[c2], da[c1]
    return False


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("YES" if solve() else "NO")
