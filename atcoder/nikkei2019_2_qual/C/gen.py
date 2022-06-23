from random import randint

n = 10000
print(n)
a = [randint(1, 10) for _ in range(n)]
b = [randint(1, 10) for _ in range(n)]
print(*a)
print(*b)
