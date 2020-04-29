import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
src = [[] for _ in range(N)]
for i in range(M):
    x, y = [int(z) - 1 for z in readline().split()]
    src[y].append(x)


def solve():
    nodes = []
    nindex = [-1] * N

    def topsort(i):
        if nindex[i] != -1:
            return
        for j in src[i]:
            topsort(j)
        nindex[i] = len(nodes)
        nodes.append(i)

    for i in range(N):
        topsort(i)

    dp = [0] * N
    for i in range(N):
        node = nodes[i]
        plen = 0
        for j in src[node]:
            k = nindex[j]
            if plen < dp[k] + 1:
                plen = dp[k] + 1
        dp[i] = plen
    return max(dp)


if __name__ == "__main__":
    print(solve())
