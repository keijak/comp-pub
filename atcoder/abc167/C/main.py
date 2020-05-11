#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M, X = map(int, readline().split())
psum = 0
books = []
for i in range(N):
    c, *a = map(int, readline().split())
    assert len(a) == M
    books.append((c, a))
    psum += c


def solve():
    U = 1 << N
    minp = psum + 1
    for i in range(U):
        sel = i
        unds = [0] * M
        k = 0
        price = 0
        while (1 << k) < U:
            if (sel >> k) & 1:
                c, a = books[k]
                price += c
                for j in range(M):
                    unds[j] += a[j]
            k += 1
        if minp > price and all(x >= X for x in unds):
            minp = price

    if minp == psum + 1:
        return -1
    return minp


if __name__ == "__main__":
    print(solve())
