import sys
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

H, W = inm()


def solve():
    sc = Counter()
    for i in range(H):
        s = ins()
        for c in s:
            sc[c] += 1
    sv = sorted(sc.values(), reverse=True)
    fours = (H // 2) * (W // 2)
    for i in range(len(sv)):
        while fours > 0 and sv[i] >= 4:
            sv[i] -= 4
            fours -= 1
    if fours > 0:
        return False
    maxtwos = 0
    if H % 2 == 1:
        maxtwos += W // 2
    if W % 2 == 1:
        maxtwos += H // 2
    for i in range(len(sv)):
        while maxtwos > 0 and sv[i] >= 2:
            sv[i] -= 2
            maxtwos -= 1
    if maxtwos > 0:
        return False

    if (H * W) % 2 == 1:
        maxodds = 1
    else:
        maxodds = 0
    return sum(sv) == maxodds


print("Yes" if solve() else "No")
