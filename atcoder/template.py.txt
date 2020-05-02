#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)

def iterate_tokens():
    for line in sys.stdin.buffer:
        for word in line.split():
            yield word

tokens = iterate_tokens()
${input_part}


def solve(${formal_arguments}):
    return


def main():
    print(solve(${actual_arguments}))


if __name__ == '__main__':
    main()
