import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
S = [ins() for _ in range(N)]


def solve():
    c = collections.Counter(S)

    print("AC x {}".format(c["AC"]))
    print("WA x {}".format(c["WA"]))
    print("TLE x {}".format(c["TLE"]))
    print("RE x {}".format(c["RE"]))


solve()
