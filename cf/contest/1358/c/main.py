import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()



def main():
    T = ni()
    for _ in range(T):
        x1, y1, x2, y2 = nm()
        print((x2 - x1) * (y2 - y1) + 1)


main()
