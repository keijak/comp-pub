import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


def solve():
    n = ini()
    a = b = 1
    y = 5
    while True:
        if y > n:
            print(-1)
            return
        a = 1
        x = 3
        while True:
            if x + y == n:
                print(a, b)
                return
            if x + y > n:
                break
            x *= 3
            a += 1
        y *= 5
        b += 1


solve()
