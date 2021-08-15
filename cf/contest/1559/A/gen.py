n = 28
a = [(1 << n) - 1] * n
for i in range(n):
    a[i] &= ~(1 << i)
    assert a[i] <= (10 ** 9)

print(1)
print(n)
print(*a)
