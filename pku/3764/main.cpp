#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;

struct Edge {
  int to;
  unsigned w;
  Edge(int to, int w) : to(to), w(w) {}
};

template <typename T, int MAX_LOG, typename D = int>
struct BinaryTrie {
 public:
  struct Node {
    Node *nxt[2];
    D exist;
    Node() : exist(0) { nxt[0] = nxt[1] = 0; }
  };

  Node *root;

  explicit BinaryTrie() : root(new Node()) {}

  explicit BinaryTrie(Node *root) : root(root) {}

  void add(const T &bit, int idx = -1, D delta = 1, T xor_val = 0) {
    root = add(root, bit, idx, MAX_LOG, delta, xor_val);
  }

  void erase(const T &bit, T xor_val = 0) { add(bit, -1, -1, xor_val); }

  Node *find(const T &bit, T xor_val = 0) {
    return find(root, bit, MAX_LOG, xor_val);
  }

  pair<T, Node *> find_max(const T &bit) {
    return find_max(root, bit, MAX_LOG);
  }

 private:
  virtual Node *clone(Node *t) { return t; }

  Node *add(Node *t, T bit, int idx, int depth, D x, T xor_val,
            bool need = true) {
    if (need) t = clone(t);
    if (depth == -1) {
      t->exist += x;
    } else {
      bool f = (xor_val >> depth) & 1;
      Node *&to = t->nxt[f ^ ((bit >> depth) & 1)];
      if (!to) to = new Node(), need = false;
      to = add(to, bit, idx, depth - 1, x, xor_val, need);
      t->exist += x;
    }
    return t;
  }

  Node *find(Node *t, T bit, int depth, T xor_val) {
    if (depth == -1) {
      return t;
    } else {
      bool f = (xor_val >> depth) & 1;
      Node *&to = t->nxt[f ^ ((bit >> depth) & 1)];
      return to ? find(to, bit, depth - 1, xor_val) : 0;
    }
  }

  pair<T, Node *> find_max(Node *t, T bit, int depth) {
    pair<T, Node *> res;
    if (depth == -1) {
      res.first = 0;
      res.second = t;
    } else {
      int on = (bit >> depth) & 1;
      Node *&to = t->nxt[1 - on];
      Node *&alt = t->nxt[on];
      if (to) {
        res = find_max(to, bit, depth - 1);
        res.first |= (1 - on) * (1 << depth);
      } else {
        res = find_max(alt, bit, depth - 1);
        res.first |= on * (1 << depth);
      }
    }
    return res;
  }
};

const int NMAX = 100005;
vector<Edge> g[NMAX];
unsigned dist[NMAX];

void dfs(int v, int p, unsigned d) {
  dist[v] = d;
  for (int i = 0; i < (int)g[v].size(); ++i) {
    const Edge &e = g[v][i];
    if (e.to == p) continue;
    dfs(e.to, v, d ^ e.w);
  }
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n - 1; ++i) {
    int u, v, w;
    scanf("%d%d%d", &u, &v, &w);
    g[u].push_back(Edge(v, w));
    g[v].push_back(Edge(u, w));
  }
  dfs(0, -1, 0);

  BinaryTrie<unsigned, 30> trie;
  for (int i = 0; i < n; ++i) {
    trie.add(dist[i]);
  }
  unsigned ans = 0;
  for (int i = 0; i < n; ++i) {
    unsigned res = trie.find_max(dist[i]).first;
    ans = max(ans, res ^ dist[i]);
  }
  printf("%u\n", ans);
}
