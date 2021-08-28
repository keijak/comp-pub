import random

print(1)
n = 100
q = 10
s = []
for i in range(n):
    b = random.randint(0, 1)
    s.append("+" if b else "-")
print(n, q)
print("".join(s))
for i in range(q):
    l = random.randint(1, n)
    r = random.randint(l, n)
    print(l, r)
