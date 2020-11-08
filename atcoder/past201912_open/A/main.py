import sys
import re

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

S = ins()


def solve():
    m = re.fullmatch(r"\d\d\d", S)
    if m:
        return 2 * int(m.group(0))
    else:
        return "error"


print(solve())
