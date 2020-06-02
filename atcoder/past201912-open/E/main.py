import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N, Q = inm()


def solve():
    fol = [set() for _ in range(N)]
    rev = [set() for _ in range(N)]

    for i in range(Q):
        c, a, *bs = inm()
        a -= 1
        if c == 1:
            b = bs[0] - 1
            if b != a:
                fol[a].add(b)
                rev[b].add(a)
        elif c == 2:
            for x in rev[a]:
                fol[a].add(x)
                rev[x].add(a)
        else:
            assert c == 3
            for x in list(fol[a]):
                for y in list(fol[x]):
                    if y != a:
                        fol[a].add(y)
                        rev[y].add(a)

    for i in range(N):
        row = []
        for j in range(N):
            row.append("Y" if j in fol[i] else "N")
        print("".join(row))


solve()
