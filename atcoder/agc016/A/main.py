import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

s = ins()


def solve():
    ss = set(s)
    ans = 10000
    ach = None
    for c in range(26):
        ch = chr(ord("a") + c)
        if ch not in ss:
            continue
        msk = sk = 0
        for i, x in enumerate(s):
            if x == ch:
                if msk < sk:
                    msk = sk
                sk = 0
            else:
                sk += 1
        if msk < sk:
            msk = sk
        if ans > msk:
            ans = msk
            ach = ch

    return ans


print(solve())
