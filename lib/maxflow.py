"""Max Flow (Ford-Fulkerson)"""


class MaxFlowGraph:
    INF = 10 ** 9

    def __init__(self, n):
        self.n = n  # number of nodes
        self.adj = [[0] * n for _ in range(n)]  # directed edges with capacity

    def add_edge(self, node_from, node_to, capacity):
        """Adds a directed edge."""
        self.adj[node_from][node_to] = capacity

    def max_flow(self, source_node, sink_node):
        """Computes the max flow."""
        total_flow = 0
        while True:
            flow, backtrack = self._dfs(source_node, sink_node)
            if not flow:
                break
            total_flow += flow
            # Reverse the edges on the path from the sink to the source.
            node = sink_node
            while node != source_node:
                prev_node = backtrack[node]
                self.adj[node][prev_node] = self.adj[prev_node][node]
                self.adj[prev_node][node] = 0
                node = prev_node
        return total_flow

    def _dfs(self, source_node, sink_node):
        stack = [(source_node, -1, MaxFlowGraph.INF)]
        backtrack = [None] * self.n
        while stack:
            node, parent_node, mincap = stack.pop()
            if backtrack[node] is not None:
                continue
            backtrack[node] = parent_node
            if node == sink_node:
                return mincap, backtrack
            for child_node in range(self.n):
                cap = self.adj[node][child_node]
                if cap > 0 and backtrack[child_node] is None:
                    stack.append((child_node, node, min(cap, mincap)))
        return 0, backtrack


class Dinic:
    def __init__(self, n, g):
        self.n = n
        self.e = [[] for _ in range(n)]
        for i in range(n):
            for j, c in g[i]:
                a = [j, c, None]
                a[2] = b = [i, 0, a]
                self.e[i].append(a)
                self.e[j].append(b)

    def bfs(self, s, t):
        self.d = d = [None] * self.n
        d[s] = 0
        k = [s]
        e = self.e
        for i in k:
            for j, c, _ in e[i]:
                if c and d[j] == None:
                    d[j] = d[i] + 1
                    k.append(j)
        return d[t] != None

    def dfs(self, v, t, f):
        if v == t:
            return f
        d = self.d
        for e in self.m[v]:
            w, c, r = e
            if c and d[v] < d[w]:
                b = self.dfs(w, t, min(f, c))
                if b:
                    e[1] -= b
                    r[1] += b
                    return b
        return 0

    def flow(self, s, t):
        l = 0
        x = 10 ** 20
        while self.bfs(s, t):
            self.m = list(map(iter, self.e))
            f = x
            while f:
                f = self.dfs(s, t, x)
                l += f
        return l
