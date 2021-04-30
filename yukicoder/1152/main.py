import sys


def decode(x):
    c2, c5 = 0, 0
    while x % 2 == 0:
        c2 += 1
        x //= 2
    while x % 5 == 0:
        c5 += 1
        x //= 5
    assert x == 1
    return [c2, c5]


def encode(p):
    c2, c5 = p
    x = 1
    for i in range(c2):
        x *= 2
    for i in range(c5):
        x *= 5
    return x


def solve():
    x1, x2 = [int(x) for x in input().split()]
    p1 = decode(x1)
    p2 = decode(x2)

    def query():
        nonlocal x1, x2, p1, p2
        x1 = encode(p1)
        print(x1, flush=True)
        if x1 == x2:
            exit(0)
        x2 = int(input())
        if x1 == x2:
            exit(0)
        p2 = decode(x2)

    P = 2 ** 7 * 5 ** 9
    while x1 != P:
        if p1[0] > 7:
            p1[0] -= 1
        elif p1[0] < 7:
            p1[0] += 1
        elif p1[1] < 9:
            p1[1] += 1
        else:
            assert False
        query()

    if x2 > x1:
        print(x2, flush=True)
        exit(0)

    if (p1[0] + p1[1]) % 2 == (p2[0] + p2[1]) % 2:
        p1[0] += 2
        query()

    while True:
        d2 = abs(p1[0] - p2[0])
        d5 = abs(p1[1] - p2[1])
        if d2 < d5:
            if p1[1] > p2[1]:
                p1[1] -= 1
            else:
                p1[1] += 1
        else:
            if p1[0] > p2[0]:
                p1[0] -= 1
            else:
                p1[0] += 1
        query()


solve()
