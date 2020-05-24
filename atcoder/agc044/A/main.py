import sys
import collections
import heapq

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()


INF = 1 << 62


def solve():
    N, A, B, C, D = nm()
    ans = D * N  # worst case
    mem = collections.defaultdict(lambda: INF)
    q = []
    heapq.heappush(q, (0, N))
    while q:
        coins, num = heapq.heappop(q)
        if mem[num] <= coins:
            continue
        mem[num] = coins
        if ans > num * D + coins:
            ans = num * D + coins
        if num == 0:
            continue
        tnum = num - (num % 2)
        heapq.heappush(q, (coins + (num - tnum) * D + A, tnum // 2))
        tnum = num + 2 - (num % 2)
        heapq.heappush(q, (coins + (tnum - num) * D + A, tnum // 2))

        tnum = num - (num % 3)
        heapq.heappush(q, (coins + (num - tnum) * D + B, tnum // 3))
        tnum = num + 3 - (num % 3)
        heapq.heappush(q, (coins + (tnum - num) * D + B, tnum // 3))

        tnum = num - (num % 5)
        heapq.heappush(q, (coins + (num - tnum) * D + C, tnum // 5))
        tnum = num + 5 - (num % 5)
        heapq.heappush(q, (coins + (tnum - num) * D + C, tnum // 5))

    return ans


def main():
    T = ni()
    for _ in range(T):
        print(solve())


main()
