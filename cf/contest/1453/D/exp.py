import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def f(n):
    ans = n
    b = 1
    while n:
        ans += n * b
        b *= 2
        n -= 1
    return ans


L = 10 ** 18


def solve():
    for i in range(1, 100):
        x = f(i)
        if x > L:
            break
        print(i, x)


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    solve()