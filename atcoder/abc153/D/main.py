import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

H = ini()


def solve(h):
    if h == 1:
        return 1
    return 1 + 2 * solve(h >> 1)


print(solve(H))
