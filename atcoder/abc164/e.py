import sys
import heapq

input = sys.stdin.readline

NCOSTS = 2501
TIME_MAX = 10 ** 15

N, M, S = map(int, input().split())
adj = [[] for _ in range(N * NCOSTS)]


def nodeid(city, silvers):
    return silvers * N + city


for i in range(M):
    u, v, a, b = map(int, input().split())
    u -= 1
    v -= 1
    for j in range(a, NCOSTS):
        adj[nodeid(u, j)].append((nodeid(v, j - a), b))
        adj[nodeid(v, j)].append((nodeid(u, j - a), b))

for i in range(N):
    # g-s rate, waiting duration
    c, d = map(int, input().split())
    for j in range(NCOSTS - c):
        adj[nodeid(i, j)].append((nodeid(i, j + c), d))


mintime = [TIME_MAX for _ in range(N * NCOSTS)]


def search():
    q = [(0, nodeid(0, min(S, NCOSTS - 1)))]
    while q:
        t, node1 = heapq.heappop(q)
        if t > mintime[node1]:
            continue
        for node2, tcost in adj[node1]:
            ntime = t + tcost
            if mintime[node2] > ntime:
                mintime[node2] = ntime
                heapq.heappush(q, (ntime, node2))


search()
for i in range(1, N):
    print(min(mintime[nodeid(i, j)] for j in range(NCOSTS)))
