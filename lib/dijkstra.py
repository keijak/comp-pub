# start: start node (0-origin)
# adj: adjacent list
def dijkstra(start, adj):
    INF = 1 << 62
    q = [(0, start)]
    dist = [INF] * N
    while q:
        d, node = heapq.heappop(q)
        if d > dist[node]:
            continue
        dist[node] = d
        for nxt, step in adj[node]:
            if dist[nxt] > d + step:
                heapq.heappush(q, (d + step, nxt))
    return dist
