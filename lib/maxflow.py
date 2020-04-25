"""Max Flow (Ford-Fulkerson)"""


class MaxFlowGraph:
    INF = 10 ** 9

    def __init__(self, n):
        self.n = n  # number of nodes
        self.adj = [[0] * n for _ in range(n)]  # directed edges with capacity

    def add_edge(self, node_from, node_to, capacity):
        self.adj[node_from][node_to] = capacity

    def max_flow(self, source_node, sink_node):
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
