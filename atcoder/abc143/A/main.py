#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)

def iterate_tokens():
    for line in sys.stdin.buffer:
        for word in line.split():
            yield word

tokens = iterate_tokens()
A = int(next(tokens))  # type: int
    B = int(next(tokens))  # type: int


def solve(A: int, B: int):
    return


def main():
    print(solve(A, B))


if __name__ == '__main__':
    main()
