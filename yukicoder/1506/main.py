import sys


def solve():
    n = int(sys.stdin.readline())
    a = [int(x) for x in sys.stdin.readline().split()]
    win = True
    for i in range(1, n):
        if not win:
            win = True
        elif a[i] == 1:
            win = False
        else:
            win = True
    return win


print(["Bob", "Alice"][solve()])
