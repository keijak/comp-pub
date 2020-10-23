import random

n = random.randint(1, 100)
s = random.randint(1, 100)
print(n, s)
a = [None] * n
for i in range(n):
    a[i] = random.randint(1, 100)
print(*a)
