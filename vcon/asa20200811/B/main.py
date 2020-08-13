import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


S = [
    "7 8 9 0",
    " 4 5 6",
    "  2 3",
    "   1",
]

a, b = inl()
p = inl()
q = inl()


def solve():
    fell = [False] * 10
    for x in p:
        fell[x] = True
        for i in range(4):
            S[i] = S[i].replace(str(x), ".")
    for x in q:
        fell[x] = True
        for i in range(4):
            S[i] = S[i].replace(str(x), "o")
    for x in range(10):
        if fell[x]:
            continue
        for i in range(4):
            S[i] = S[i].replace(str(x), "x")

    return "\n".join(S)


print(solve())
