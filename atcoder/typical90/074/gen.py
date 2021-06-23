import random

n = 4
s = ""
for i in range(n):
    g = random.randint(0, 2)
    s += chr(ord("a") + g)
print(n)
print(s)
