n = int(input())

a = [None] * (2 * n)
for i in range(1, n + 1):
    a[2 * i - 1] = i

while len(a) > n + 1:
    x = a.pop()
    if x is None:
        continue
    for i in range(len(a) - 1, 0, -1):
        if a[i] is None:
            a[i] = x
            break

print(*a[1:])
