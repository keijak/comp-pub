import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    winning = False
    while n > 1:
        if winning and n % 2 == 0:
            n -= 1
            continue
        n //= 2
        winning = not winning
    return winning


print("Takahashi" if solve() else "Aoki")
