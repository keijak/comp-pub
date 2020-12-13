import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    L = ini()
    memo = [[None] * 13 for i in range(L + 1)]
    for i in range(L + 1):
        memo[i][1] = 1

    def f(length, num):
        if num == 1:
            return 1
        if length <= 1:
            return 0
        x = memo[length][num]
        if x is not None:
            return x
        res = 0
        for i in range(1, length):
            res += f(length - i, num - 1)
        memo[length][num] = res
        return res

    return f(L, 12)


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())
