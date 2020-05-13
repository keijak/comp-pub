#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

S = readline().decode("utf-8").rstrip()
sp = [int(h == "p") for h in S]


def solve():
    g = p = 0
    score = 0
    for is_p in sp:
        if g == p:
            hand = 0
            g += 1
        else:
            hand = 1
            p += 1
        if hand > is_p:
            score += 1
        elif hand < is_p:
            score -= 1
    return score


if __name__ == "__main__":
    print(solve())
