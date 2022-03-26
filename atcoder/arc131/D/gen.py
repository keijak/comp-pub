import random

n = 100
m = 20
d = random.randint(1, 10**6)
print(n, m, d)
r = set([0])
s = set()
for i in range(m):
    while True:
        y = random.randint(1, 10**6)
        if y not in r:
            r.add(y)
            break
    while True:
        z = random.randint(1, 10**6)
        if z not in s:
            s.add(z)
            break
print(*sorted(r))
print(*sorted(s, reverse=True))
