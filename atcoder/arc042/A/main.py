import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n, m = inl()
    a = [ini() for _ in range(m)]
    used = [False] * n
    ans = []
    for x in a[::-1]:
        if used[x - 1]:
            continue
        used[x - 1] = True
        ans.append(x)
    for i in range(n):
        if not used[i]:
            ans.append(i + 1)
    return ans


print(*solve(), sep="\n")
