#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)

def iterate_tokens():
    for line in sys.stdin.buffer:
        for word in line.split():
            yield word

tokens = iterate_tokens()
N = int(next(tokens))  # type: int
    A = [int(next(tokens)) for _ in range(N)]  # type: "List[int]"


def solve(N: int, A: "List[int]"):
    return


def main():
    print(solve(N, A))


if __name__ == '__main__':
    main()
