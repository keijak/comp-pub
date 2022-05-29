from random import randint

n = 2 * 10**5
u = n * (n - 1) // 2
l = randint(1, u)
r = randint(l, u)

print(n, l, r)
