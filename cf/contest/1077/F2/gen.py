import random

n = random.randint(500, 1000)
k = random.randint(100, n)
x = random.randint(200, n)
print(n, k, x)

a = [None] * n
for i in range(n):
    a[i] = random.randint(1, 10 ** 9)
print(*a)
