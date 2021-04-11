import random
import sys


def gen(k):
    sa = "".join([chr(ord("a") + random.randint(0, 25)) for _ in range(10)])
    sb = "".join([chr(ord("a") + random.randint(0, 25)) for _ in range(10)])
    sc = "".join([chr(ord("a") + random.randint(0, 25)) for _ in range(10)])

    with open(f"./test/my2-{k}.in", "w") as f:
        print(sa, file=f)
        print(sb, file=f)
        print(sc, file=f)


for k in range(100):
    gen(k)
