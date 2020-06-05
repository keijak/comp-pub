import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()


def solve():
    s = ins()
    t = ins()
    assert len(s) == len(t) == 3
    if s == t:
        return "same"
    if s.lower() == t.lower():
        return "case-insensitive"
    return "different"


print(solve())

