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
    rf = [True for _ in range(n)]
    rf[n - 1] = False
    rb = [True for _ in range(n)]
    rb[0] = False
    for i in range(n):
        if allow_out[i] == "N":
            rf[i] = rb[i] = False
        if allow_in[i] == "N":
            if i - 1 >= 0:
                rf[i - 1] = False
            if i + 1 < n:
                rb[i + 1] = False

    P = [[False for _ in range(n)] for _ in range(n)]
    for i in range(n):
        P[i][i] = True
        for j in range(i - 1, -1, -1):
            if not rb[j + 1]:
                break
            P[i][j] = True
        for j in range(i + 1, n):
            if not rf[j - 1]:
                break
            P[i][j] = True
    for i in range(n):
        print("".join(["Y" if ok else "N" for ok in P[i]]))


def main():
    T = ini()
    for i in range(T):
        print(f"Case #{i+1}: ")
        solve()


main()
