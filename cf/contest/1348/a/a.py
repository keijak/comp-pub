import sys
import collections

readline = sys.stdin.buffer.readline


def solve(n):
    h = n // 2
    bs = [2 ** i for i in range(1, n + 1)]
    a = sum(bs[: h - 1]) + bs[-1]
    b = sum(bs) - a
    return abs(a - b)


def main():
    T = int(readline())
    for _ in range(T):
        n = int(readline())
        print(solve(n))


if __name__ == "__main__":
    main()
