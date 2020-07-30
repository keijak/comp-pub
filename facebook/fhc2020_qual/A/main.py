import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    allow_in = ins()
    allow_out = ins()
    P = [[False for _ in range(n)] for _ in range(n)]
    for i in range(n):
        P[i][i] = True
    for i in range(1, n):
        P[i][i - 1] = allow_in[i - 1] == "Y" and allow_out[i] == "Y"
        P[i - 1][i] = allow_in[i] == "Y" and allow_out[i - 1] == "Y"
    for k in range(n):
        for i in range(n):
            for j in range(n):
                P[i][j] = P[i][j] or (P[i][k] and P[k][j])
    for i in range(n):
        print("".join(["Y" if ok else "N" for ok in P[i]]))


def main():
    T = ini()
    for i in range(T):
        print(f"Case #{i+1}: ")
        solve()


main()
