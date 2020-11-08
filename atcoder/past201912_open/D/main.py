import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N = ini()
ctr = collections.Counter(map(int, sys.stdin.readlines()))


def solve():
    zero = two = None
    for i in range(1, N + 1):
        c = ctr[i]
        if c == 1:
            continue
        elif c == 0:
            assert zero is None
            zero = i
        elif c == 2:
            assert two is None
            two = i

    if zero is None and two is None:
        print("Correct")
    else:
        assert zero is not None
        assert two is not None
        print(two, zero)


solve()
