#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)

def iterate_tokens():
    for line in sys.stdin.buffer:
        for word in line.split():
            yield word

tokens = iterate_tokens()
N = int(next(tokens))  # type: int
    S = next(tokens)  # type: str


def solve(N: int, S: str):
    return


def main():
    print(solve(N, S))


if __name__ == '__main__':
    main()
