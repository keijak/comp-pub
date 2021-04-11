import random
import sys


def gen(k):
    x = random.randint(1, 10 ** 9)
    y = random.randint(1, 10 ** 9)
    z = x + y

    p = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j"]
    assert len(p) == 10
    random.shuffle(p)

    d = {}
    for i, c in enumerate(p):
        d[str(i)] = c
    sx = str(x)
    sa = "".join(map(lambda c: d[c], sx))

    sy = str(y)
    sb = "".join(map(lambda c: d[c], sy))

    sz = str(z)
    sc = "".join(map(lambda c: d[c], sz))

    with open(f"./test/my-{k}.in", "w") as f:
        print(sa, file=f)
        print(sb, file=f)
        print(sc, file=f)
    with open(f"./test/my-{k}.out", "w") as f:
        print(sx, file=f)
        print(sy, file=f)
        print(sz, file=f)


for k in range(10):
    gen(k)
