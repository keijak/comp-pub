from random import randint
from random import shuffle
from itertools import chain

t = 1000
print(4 * t)
for i in range(t):
    ax = randint(-(10 ** 8), 10 ** 8)
    ay = randint(-(10 ** 8), 10 ** 8)

    zs = [
        [(ax, ay), (ax + 1, ay), (ax, ay + 1)],
        [(ax, ay), (ax - 1, ay), (ax, ay + 1)],
        [(ax, ay), (ax + 1, ay), (ax, ay - 1)],
        [(ax, ay), (ax - 1, ay), (ax, ay - 1)],
    ]

    for z in zs:
        shuffle(z)
        print(*chain(*z))
