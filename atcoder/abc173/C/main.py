import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

H, W, K = inm()
grid = [ins() for _ in range(H)]


def solve():
    ans = 0
    for i in range(1 << H):
        for j in range(1 << W):
            cnt = 0
            for r in range(H):
                if i & (1 << r):
                    continue
                for c in range(W):
                    if j & (1 << c):
                        continue
                    if grid[r][c] == "#":
                        cnt += 1
            if cnt == K:
                ans += 1

    return ans


print(solve())
