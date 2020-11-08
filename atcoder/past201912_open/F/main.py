import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

S = ins()


def solve():
    ps = []
    st = 0
    i = 1
    while i < len(S):
        if S[i].isupper():
            w = S[st : i + 1]
            ps.append((w.lower(), w))
            st = i + 1
            i = st + 1
        else:
            i += 1
    ps.sort()
    return "".join([w for (_, w) in ps])


print(solve())
