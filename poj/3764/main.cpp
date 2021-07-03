#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;

struct Edge {
  int to;
  unsigned w;
  Edge(int to, int w) : to(to), w(w) {}
};

template<typename T = unsigned, int kBitWidth = 31>
struct BinaryTrie {
 public:
  struct Node;
  typedef Node *NodePtr;

  struct Node {
    int leaf_count;
    NodePtr child[2];
    Node() : leaf_count(0) {
      child[0] = child[1] = 0;
    }
  };
  NodePtr root_;  // The root node.

  BinaryTrie() : root_(0) {}

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  void insert(T val) { root_ = insert_internal(root_, val); }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  void erase_one(T val) { root_ = erase_internal(root_, val); }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns the minimum index i s.t. trie[i] >= val.
  int lower_bound(T val) const { return count_less(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less(root_, val + 1); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    return get_internal(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    const int i = size() - k - 1;
    return (*this)[i];
  }

  // Counts the number of elements that are equal to `val`.
  // Note: BinaryTrie is a multiset.
  int count(T val) const {
    if (!root_) return 0;
    NodePtr t = root_;
    for (int i = kBitWidth - 1; i >= 0; i--) {
      t = t->child[(val >> T(i)) & 1];
      if (!t) return 0;
    }
    return t->leaf_count;
  }

  void xor_all(T xor_mask) {
    if (root_) root_->lazy_mask ^= xor_mask;
  }

 private:

  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    if (!t) t = new Node();
    t->leaf_count += 1;
    if (b < 0) return t;
    bool f = (val >> b) & 1;
    t->child[f] = insert_internal(t->child[f], val, b - 1);
    return t;
  }

  NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    t->leaf_count -= 1;
    if (t->leaf_count == 0) {
      delete t;
      return 0;
    }
    if (b < 0) return t;
    push_down(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = erase_internal(t->child[f], val, b - 1);
    return t;
  }

  T get_min(NodePtr t, T xor_mask, int b = kBitWidth - 1) const {
    if (b < 0) return 0;
    bool f = (xor_mask >> b) & 1;
    f ^= !t->child[f];
    return get_min(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  T get_internal(NodePtr t, int k, int b = kBitWidth - 1) const {
    if (b < 0) return 0;
    int m = t->child[0] ? t->child[0]->leaf_count : 0;
    return k < m ? get_internal(t->child[0], k, b - 1)
                 : get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  int count_less(NodePtr t, T val, int b = kBitWidth - 1) const {
    if (!t || b < 0) return 0;
    bool f = (val >> b) & 1;
    return (f && t->child[0] ? t->child[0]->leaf_count : 0) +
        count_less(t->child[f], val, b - 1);
  }
};

const int NMAX = 100005;
vector<Edge> g[NMAX];
unsigned dist[NMAX];

struct DfsState {
  int v;
  int p;
  unsigned d;
  DfsState() {}
  DfsState(int v, int p, int d) : v(v), p(p), d(d) {}
};
DfsState dfs_stack[NMAX];

void dfs(int v, int p, unsigned d) {
  dist[v] = d;
  for (int i = 0; i < (int) g[v].size(); ++i) {
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
  int sp = 0;
  dfs_stack[sp++] = DfsState(0, -1, 0);
  while (sp > 0) {
    DfsState s = dfs_stack[--sp];
    dist[s.v] = s.d;
    int sz = g[s.v].size();
    for (int i = 0; i < sz; ++i) {
      const Edge &e = g[s.v][i];
      if (e.to == s.p) continue;
      dfs_stack[sp++] = DfsState(e.to, s.v, s.d ^ e.w);
    }
  }

  BinaryTrie<unsigned, 31> trie;
  for (int i = 0; i < n; ++i) {
    trie.insert(dist[i]);
  }
  unsigned ans = 0;
  for (int i = 0; i < n; ++i) {
    unsigned d = trie.max_element(dist[i]);
    ans = max(ans, d ^ dist[i]);
  }
  printf("%u\n", ans);
}
