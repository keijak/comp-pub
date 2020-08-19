import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()

md1 = inl()
md2 = inl()


def solve():
    return md1[0] < md2[0]


print(1 if solve() else 0)
