import random

n = 10
k = random.randint(2, 9)

a = list(range(1, n + 1))
random.shuffle(a)
print(n, k)
print(*sorted(a[:k]))
