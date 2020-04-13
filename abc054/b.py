n, m = map(int, input().split())
a, b = [], []
for i in range(n):
    s = input()
    assert len(s) == n
    a.append(s)
for j in range(m):
    s = input()
    assert len(s) == m
    b.append(s)


def scan(i1, j1):
    for i2 in range(m):
        if a[i1+i2][j1:j1+m] != b[i2]:
            return False
    return True


def solve():
    for i1 in range(n-m+1):
        for j1 in range(n-m+1):
            if scan(i1, j1):
                return True
    return False


print('Yes' if solve() else 'No')
