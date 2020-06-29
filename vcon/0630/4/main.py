import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N, R = inm()
S = list(ins())


def solve():
    last = -1
    for i in range(N):
        if S[i] == ".":
            last = i
    if last == -1:
        return 0

    t = 0
    for i in range(N):
        if S[i] == "." or i >= last - R + 1:
            for j in range(i, min(i + R, last + 1)):
                S[j] = "o"
            t += 1
            if i + R >= last + 1:
                break
        t += 1

    return t


print(solve())
