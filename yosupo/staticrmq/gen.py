import random

n = 1000
q = 1000
print(n, q)
a = [random.randint(0, 10 ** 9) for i in range(n)]
print(*a)
for i in range(q):
    l = random.randint(0, n - 1)
    r = random.randint(l + 1, n)
    print(l, r)
