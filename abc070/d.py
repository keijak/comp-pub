"""Dijkstra on a tree"""
import sys
import heapq

input = sys.stdin.readline


def main():
    N = int(input())
    nb = [[] for _ in range(N)]
    for i in range(N - 1):
        a, b, c = map(int, input().split())
        a -= 1
        b -= 1
        nb[a].append((c, b))
        nb[b].append((c, a))
    for x in nb:
        x.sort()

    Q, K = map(int, input().split())
    K -= 1

    dist = [None] * N
    h = [(0, K, -1)]
    while h:
        d0, node, parent = heapq.heappop(h)
        dist[node] = d0
        for d1, child in nb[node]:
            if child != parent:
                heapq.heappush(h, (d0 + d1, child, node))

    for i in range(Q):
        x, y = map(int, input().split())
        # print(f"{x}->K = {dist[x]}, K->{y} = {dist[y]}")
        print(dist[x - 1] + dist[y - 1])


if __name__ == "__main__":
    main()
