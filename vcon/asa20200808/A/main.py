import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()


def solve():
    s = sum(A)
    cnt = 0
    for x in A:
        if x > 0:
            cnt += 1

    return (s + cnt - 1) // cnt


print(solve())
