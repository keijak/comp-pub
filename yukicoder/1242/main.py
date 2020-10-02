import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()


def find_item(x, a):
    if x <= 0:
        return False
    n = len(a)
    for i in range(n - 1, -1, -1):
        if a[i] == x:
            return True
        if a[i] < x:
            break
    return False


def insert_item(x, a):
    if x <= 0:
        return
    if find_item(x, a):
        return
    a.append(x)
    n = len(a)
    for i in range(n - 2, -1, -1):
        if a[i] < a[i + 1]:
            break
        a[i], a[i + 1] = a[i + 1], a[i]


def solve():
    n, k = inl()
    a = inl()
    a.sort()
    deads = set()
    while a:
        x = a.pop()
        if x == 1:
            return False
        if all(x + i in deads for i in range(1, 6)):
            return False
        deads.add(x)
        if find_item(x - 1, a):
            insert_item(x - 4, a)
        if find_item(x - 3, a):
            insert_item(x - 5, a)
        if find_item(x - 5, a):
            insert_item(x - 6, a)
    return True


print("Yes" if solve() else "No")
