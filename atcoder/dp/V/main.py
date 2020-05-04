#!/usr/bin/env python3
import collections
import sys

sys.setrecursionlimit(10 ** 8)


def solve(N: int, M: int, adj):
    sub = collections.defaultdict(lambda: 0)

    def dfs(node, parent):
        if parent >= 0 and (node*N+parent) in sub:
            return sub[node*N+parent]
        r = 1
        for child in adj[node]:
            if child == parent:
                continue
            r = r * (1 + dfs(child, node)) % M
        if parent >= 0:
            sub[node*N+ parent] = r
        return r

    ans = [0] * N
    for i in range(N):
        ans[i] = dfs(i, -1)
    return ans


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    M = int(next(tokens))  # type: int
    adj = [[] for _ in range(N)]
    for i in range(N - 1):
        x = int(next(tokens)) - 1
        y = int(next(tokens)) - 1
        adj[x].append(y)
        adj[y].append(x)
    print(*solve(N, M, adj), sep="\n")


if __name__ == "__main__":
    main()
