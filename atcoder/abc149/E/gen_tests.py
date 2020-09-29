import random

n = random.randint(1, 500)
m = random.randint(1, n * n)
print(n, m)
a = []
for i in range(n):
    x = random.randint(10, 100000)
    a.append(x)
print(*a)
