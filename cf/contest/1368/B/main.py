import sys
import itertools

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

K = ini()


def strategy1():
    base = 1
    for i in range(1, 40):
        if i ** 10 >= K:
            base = i
            break
    prod = (base - 1) ** 10
    assert prod < K
    x = 0
    for i in range(10):
        prod //= base - 1
        prod *= base
        if prod >= K:
            x = i + 1
            break
    assert prod >= K
    count = (base - 1) * (10 - x) + base * x

    def output():
        s = "codeforces"
        rep = [base - 1] * (10 - x) + [base] * x
        for ch, cnt in zip(s, rep):
            print(ch * cnt, sep="", end="")

    return count, output


def solve():
    if K == 1:
        print("codeforces")
        return
    n1, out1 = strategy1()
    n2, out2 = strategy2()


solve()
