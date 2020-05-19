import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, M = nm()
adj = [[False] * N for _ in range(N)]
for i in range(M):
    x, y = nm()
    x -= 1
    y -= 1
    adj[x][y] = adj[y][x] = True


def solve():
    L = 1 << N
    maxsize = 0
    for s in range(L):
        group = []
        ok = True
        for j in range(N):
            if (s >> j) & 1:
                aj = adj[j]
                if any(not aj[z] for z in group):
                    ok = False
                    break
                group.append(j)
        if ok and maxsize < len(group):
            maxsize = len(group)
            # print(f">> maxsize={maxsize}, group={group}")
    return maxsize


if __name__ == "__main__":
    print(solve())
