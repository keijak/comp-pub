import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    s = ins()
    if n % 2 == 1:
        return -1
    m = n // 2
    cnt = 0
    for i in range(m):
        if s[i] != s[m + i]:
            cnt += 1
    return cnt


print(solve())
