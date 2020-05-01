#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10 ** 8)
INF = 1 << 62


def solve(N: int, a: "List[int]"):
    dp = [[INF] * N for _ in range(N + 1)]
    cumsize = [0] * (N + 1)
    for i in range(1, N + 1):
        cumsize[i] = cumsize[i - 1] + a[i - 1]

    def mincost(length, head):
        if head + length > N:
            return INF
        if dp[length][head] < INF:
            return dp[length][head]
        base_cost = cumsize[head + length] - cumsize[head]
        if length == 1:
            dp[length][head] = 0
            return 0
        cost = INF
        for j in range(1, length):
            if head + j >= N:
                continue
            c = mincost(j, head) + mincost(length - j, head + j)
            if cost > c:
                cost = c
        cost += base_cost
        dp[length][head] = cost
        return cost

    return mincost(N, 0)


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    a = [int(next(tokens)) for _ in range(N)]  # type: "List[int]"
    print(solve(N, a))


if __name__ == "__main__":
    main()
