import heapq

COST_MAX = 2501
TIME_MAX = 10 ** 15

N, M, S = map(int, input().split())
fare_adj = [[COST_MAX] * N for _ in range(N)]
time_adj = [[TIME_MAX] * N for _ in range(N)]
fare_max = 0
fare_sum = 0
for i in range(M):
    u, v, a, b = map(int, input().split())
    u -= 1
    v -= 1
    fare_adj[u][v] = fare_adj[v][u] = a
    time_adj[u][v] = time_adj[v][u] = b
    fare_sum += a
    fare_max = max(fare_max, a)

exchanges = []
for i in range(N):
    # g-s rate, waiting duration
    c, d = map(int, input().split())
    exchanges.append((c, d))


mintime = [[TIME_MAX] * (fare_sum + 1) for _ in range(N)]


def search():
    q = [(0, 0, S)]
    while q:
        t, loc, silvers = heapq.heappop(q)
        silvers = min(silvers, fare_sum)
        if t >= mintime[loc][silvers]:
            continue
        mintime[loc][silvers] = t
        while silvers <= fare_sum:
            for nloc in range(N):
                if nloc == loc:
                    continue
                ntime = t + time_adj[loc][nloc]
                nsilvers = min(silvers - fare_adj[loc][nloc], fare_sum)
                if nsilvers >= 0 and ntime < mintime[nloc][nsilvers]:
                    heapq.heappush(q, (ntime, nloc, nsilvers))
            rate, wait = exchanges[loc]
            t += wait
            silvers += rate


search()
for t in range(1, N):
    print(min(mintime[t]))
