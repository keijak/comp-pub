import sys
import random

try:
    import frosch

    frosch.hook()
except ImportError:
    pass
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

INFVAL = 10 ** 9 + 5


def solve():
    n, start, x = inl()
    d = dict()
    rev = list()
    unknowns = list(range(1, n + 1))
    unknowns.remove(start)
    over_minval = INFVAL
    lastidx = None
    for i in range(1700):
        if not unknowns:
            break
        idx = random.choice(unknowns)
        print(f"? {idx}")
        sys.stdout.flush()
        unknowns.remove(idx)
        val, nex = map(int, input().split())
        if nex in unknowns:
            unknowns.remove(nex)
        if val >= x:
            over_minval = min(over_minval, val)
        d[idx] = (val, val, nex)
        rev.append((val, idx, nex))
        if nex == -1:
            lastidx = nex
        elif nex not in d:
            d[nex] = (val + 1, None, None)
    rev.sort()
    head = start
    if rev and x < rev[0][0]:
        over_minval = rev[0][0]
    else:
        for val, idx, nex in rev:
            if val >= x:
                over_minval = val
                break
            head = idx

    def query():
        nonlocal head, over_minval
        if head is None or head == lastidx or head == -1:
            return True
        while head in d:
            _, val, nex = d[head]
            if nex is None:
                break
            if val is not None and val >= x:
                over_minval = min(over_minval, val)
                return True
            if nex == -1:
                return True
            head = nex

        print(f"? {head}")
        sys.stdout.flush()
        val, nex = map(int, input().split())
        d[head] = (val, val, nex)
        if val >= x:
            over_minval = min(over_minval, val)
        head = nex
        return False

    for i in range(299):
        done = query()
        if done:
            break

    if over_minval == INFVAL:
        over_minval = -1
    print(f"! {over_minval}")


solve()
