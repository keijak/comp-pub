#!/usr/bin/env python3
import sys

MOD = 1000000007  # type: int


def solve(N: int, K: int, a: "List[int]"):
    return


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    K = int(next(tokens))  # type: int
    a = [int(next(tokens)) for _ in range(N)]  # type: "List[int]"
    solve(N, K, a)


if __name__ == "__main__":
    main()
