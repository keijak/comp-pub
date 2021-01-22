import random

n, k, m = 1000, 42, 1000
print(n, k, m)

cnt = 0
while cnt < n:
    x = random.randint(1, 3)
    r = random.randint(2, 500)
    if cnt + r > n:
        r = n - cnt
    cnt += r
    for i in range(r):
        print(x, end=" ")
print()
