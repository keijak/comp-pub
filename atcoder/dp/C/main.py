import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
options = [None] * N
for i in range(N):
    options[i] = tuple(int(x) for x in readline().split())


def solve():
    da, db, dc = 0, 0, 0
    for i in range(N):
        xa, xb, xc = options[i]
        xa += da
        xb += db
        xc += dc
        da, db, dc = max(xb, xc), max(xa, xc), max(xa, xb)
    print(max(da, db, dc))


if __name__ == "__main__":
    solve()
