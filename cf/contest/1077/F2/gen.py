import random

n = random.randint(4000, 5000)
k = random.randint(3000, n)
x = random.randint(3000, n)
print(n, k, x)

a = [None] * n
for i in range(n):
    a[i] = random.randint(1, 10 ** 9)
print(*a)
