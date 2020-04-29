import sys
import numpy as np
import numba
from numba.typed import List, Dict

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
src = List()
for i in range(N):
    src.append(List.empty_list(numba.i8))
for i in range(M):
    x, y = [int(z) - 1 for z in readline().split()]
    src[y].append(x)


@numba.njit(cache=True)
def calc(N, srclist, nodes, nindex):
    dp = np.zeros(N, dtype=np.int64)
    for i in range(N):
        node = nodes[i]
        plen = 0
        for j in srclist[node]:
            k = nindex[j]
            if plen < dp[k] + 1:
                plen = dp[k] + 1
        dp[i] = plen
    return dp.max()


def solve():
    nodes = List()
    nindex = -1 * np.ones(N, dtype=np.int64)

    def topsort(i):
        if nindex[i] != -1:
            return
        for j in src[i]:
            topsort(j)
        nindex[i] = len(nodes)
        nodes.append(i)

    for i in range(N):
        topsort(i)

    return calc(N, src, nodes, nindex)


if __name__ == "__main__":
    print(solve())
