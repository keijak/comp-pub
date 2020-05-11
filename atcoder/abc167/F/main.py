#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N = int(readline())
S = [None] * N
LR = [None] * N
lonly, ronly, none, both = [], [], [], []
for i in range(N):
    S[i] = readline().rstrip()
    l = r = 0
    for c in S[i]:
        if c == "(":
            l += 1
        elif c == ")":
            if l > 0:
                l -= 1
            else:
                r += 1
    LR[i] = (r - l, l, r)
    if l == 0 and r == 0:
        none.append(i)
    elif r == 0:
        lonly.append(i)
    elif l == 0:
        ronly.append(i)
    else:
        both.append((r - l, i))

both.sort()


def solve():
    lstack = 0
    for i in lonly:
        l, _ = LR[i]
        lstack += l
    for i in both:
        l, r = LR[i]

    return


if __name__ == "__main__":
    print(solve())
