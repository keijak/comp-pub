import random

n = random.randint(100, 300)
m = random.randint(100, 300)
print(n, m)

C = list(range(1, 1000 + 1))
random.shuffle(C)
for i in range(n):
    print(C[i])
