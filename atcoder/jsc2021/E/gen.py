import random

k = random.randint(0, 10)
n = random.randint(100, 100000)
s = "".join([chr(ord("a") + random.randint(0, 25)) for _ in range(n)])
print(k)
print(s)
