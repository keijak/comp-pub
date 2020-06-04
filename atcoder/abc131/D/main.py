import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()

N = ini()
tasks = []
for i in range(N):
    a, b = inm()
    tasks.append((b, a))
tasks.sort()


def solve():
    t = 0
    for b, a in tasks:
        if t + a > b:
            return False
        t += a
    return True


print("Yes" if solve() else "No")
