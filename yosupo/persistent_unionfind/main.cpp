#include <bits/stdc++.h>

using namespace std;

template <typename T, int K = 20>
class PersistentArray {
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // std::shared_ptr<Node>;

  struct Node {
    std::optional<T> val;
    std::unique_ptr<std::array<NodePtr, K>> children;
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  std::optional<T> operator[](int idx) const { return do_get(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(do_set(idx, val, root_));
  }

 private:
  static std::optional<T> do_get(int idx, const NodePtr &node) {
    if (node == nullptr) return std::nullopt;
    if (idx == 0) return node->val;
    if (node->children == nullptr) return std::nullopt;
    const NodePtr &child = (*node->children)[idx % K];
    if (child == nullptr) return std::nullopt;
    return do_get(idx / K, child);
  }

  static NodePtr do_set(int idx, T val, const NodePtr &node) {
    NodePtr res{new Node()};
    if (node != nullptr) {
      res->val = node->val;
      if (node->children != nullptr) {
        res->children.reset(new std::array<NodePtr, K>(*node->children));
      }
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      if (res->children == nullptr) {
        res->children.reset(new std::array<NodePtr, K>());
      }
      (*res->children)[idx % K] =
          do_set(idx / K, std::move(val), (*res->children)[idx % K]);
    }
    return res;
  }

 private:
  NodePtr root_;
};

class PersistentUnionFind {
 public:
  explicit PersistentUnionFind(PersistentArray<int> par)
      : parent_{std::move(par)} {}
  PersistentUnionFind() : parent_{} {}
  PersistentUnionFind(const PersistentUnionFind &) = default;
  PersistentUnionFind(PersistentUnionFind &&) = default;
  PersistentUnionFind &operator=(const PersistentUnionFind &) = default;
  PersistentUnionFind &operator=(PersistentUnionFind &&) = default;

  PersistentUnionFind unite(int x, int y) const {
    x = find(x);
    y = find(y);
    if (x == y) return *this;
    // Ensure x is bigger than y.
    int x_size = -parent_[x].value_or(-1);
    int y_size = -parent_[y].value_or(-1);
    if (x_size < y_size) {
      std::swap(x, y);
      std::swap(x_size, y_size);
    }
    return PersistentUnionFind(parent_.set(x, -(x_size + y_size)).set(y, x));
  }

  int find(int x) const {
    const std::optional<int> &par = parent_[x];
    if (not par or *par < 0) return x;
    return find(*par);
  }

  bool same(int x, int y) const { return find(x) == find(y); }

  int size(int x) const {
    int root = find(x);
    return -(parent_[root].value_or(-1));
  }

 private:
  PersistentArray<int> parent_;
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<PersistentUnionFind> puf(q + 1);
  for (int i = 1; i <= q; ++i) {
    int t, k, u, v;
    cin >> t >> k >> u >> v;
    if (t == 0) {
      puf[i] = puf[k + 1].unite(u, v);
    } else {
      cout << int(puf[k + 1].same(u, v)) << "\n";
    }
  }
}
