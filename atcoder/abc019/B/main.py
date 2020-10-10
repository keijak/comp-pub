import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    s += "!"
    n = len(s)
    c = 1
    p = s[0]
    ans = []
    for i in range(1, n):
        if s[i] == p:
            c += 1
        else:
            ans.append(f"{p}{c}")
            c = 1
            p = s[i]

    return "".join(ans)


print(solve())
