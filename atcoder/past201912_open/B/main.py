import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N = ini()
A = list(map(int, sys.stdin.readlines()))
assert len(A) == N


def solve():
    p = A[0]
    for i in range(1, N):
        a = A[i]
        if a == p:
            print("stay")
        elif a < p:
            print("down {}".format(p - a))
        else:
            print("up {}".format(a - p))
        p = a


solve()
