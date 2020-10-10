import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    k = ini()
    n = len(s)
    ss = set()
    for i in range(n):
        if i + k > n:
            break
        ss.add(s[i : i + k])

    return len(ss)


print(solve())
