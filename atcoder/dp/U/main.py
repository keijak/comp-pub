#!/usr/bin/env python3
import sys


def solve(N: int, a: "List[List[int]]"):
    grp = [[] for _ in range(N)]
    gsum = [0 for _ in range(N)]
    amax = 0

    def dfs(idx):
        nonlocal amax
        if idx == N:
            s = sum(gsum)
            if amax < s:
                amax = s
                # print(f"sum={s}, grp({grp})", file=sys.stderr)
            return
        for i in range(idx + 1):
            s = sum(a[idx][j] for j in grp[i])
            gsum[i] += s
            grp[i].append(idx)
            dfs(idx + 1)
            grp[i].pop()
            gsum[i] -= s
        return

    dfs(0)
    return amax


def main():
    def iterate_tokens():
        for line in sys.stdin:
            for word in line.split():
                yield word

    tokens = iterate_tokens()
    N = int(next(tokens))  # type: int
    a = [
        [int(next(tokens)) for _ in range(N)] for _ in range(N)
    ]  # type: "List[List[int]]"
    print(solve(N, a))


if __name__ == "__main__":
    main()
