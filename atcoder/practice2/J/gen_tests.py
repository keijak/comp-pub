import random

n = 2 * 10 ** 5
q = 2 * 10 ** 5
print(n, q)
A = [random.randint(0, 10 ** 7) for _ in range(n)]
print(*A, sep=" ")
del A
for i in range(q):
    t = random.randint(1, 3)
    if t == 2:
        l = random.randint(1, n)
        r = random.randint(l, n)
        print(t, l, r)
    else:
        x = random.randint(1, n)
        v = random.randint(0, 10 ** 7)
        print(t, x, v)
