import sys
from collections import Counter

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    name = ins()
    kit = ins()
    nc = Counter(name)
    kc = Counter(kit)
    ans = 0
    for k, v in nc.items():
        x = kc[k]
        if x == 0:
            return -1
        y = (v + x - 1) // x
        ans = max(ans, y)
    return ans


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
