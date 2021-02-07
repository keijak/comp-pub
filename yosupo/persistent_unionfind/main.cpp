#include <bits/stdc++.h>

using namespace std;

template <typename T, int K = 20>
struct PersistentArray {
  struct Node;
  // No memory release by default.
  using NodePtr = std::shared_ptr<Node>;

  struct Node {
    std::optional<T> val;
    std::array<NodePtr, K> ch;
    Node() : val{}, ch{} {}
  };

  explicit PersistentArray(NodePtr root) : root_{std::move(root)} {}
  PersistentArray() : root_{} {}
  PersistentArray(const PersistentArray &) = default;
  PersistentArray(PersistentArray &&) = default;
  PersistentArray &operator=(const PersistentArray &) = default;
  PersistentArray &operator=(PersistentArray &&) = default;

  std::optional<T> operator[](int idx) const { return get_(idx, root_); }

  PersistentArray<T, K> set(int idx, T val) const {
    return PersistentArray<T, K>(set_(idx, val, root_));
  }

 private:
  static std::optional<T> get_(int idx, const NodePtr &node) {
    if (node == nullptr) return std::nullopt;
    if (idx == 0) return node->val;
    const NodePtr &child = node->ch[idx % K];
    if (child == nullptr) return std::nullopt;
    return get_(idx / K, child);
  }

  static NodePtr set_(int idx, T val, const NodePtr &node) {
    NodePtr res{new Node()};
    if (node != nullptr) {
      res->val = node->val;
      std::copy(node->ch.begin(), node->ch.end(), res->ch.begin());
    }
    if (idx == 0) {
      res->val = std::move(val);
    } else {
      res->ch[idx % K] = set_(idx / K, std::move(val), res->ch[idx % K]);
    }
    return res;
  }

  NodePtr root_;
};

class PersistentUnionFind {
  PersistentArray<int> data_;

 public:
  PersistentUnionFind() : data_{} {}
  explicit PersistentUnionFind(PersistentArray<int> data)
      : data_{std::move(data)} {}

  PersistentUnionFind unite(int x, int y) const {
    x = find(x);
    y = find(y);
    if (x == y) return *this;
    // Ensure x is bigger than y.
    int x_size = -data_[x].value_or(-1);
    int y_size = -data_[y].value_or(-1);
    if (x_size < y_size) {
      std::swap(x, y);
      std::swap(x_size, y_size);
    }
    return PersistentUnionFind(data_.set(x, -(x_size + y_size)).set(y, x));
  }

  int find(int x) const {
    const std::optional<int> &par = data_[x];
    if (not par or *par < 0) return x;
    return find(*par);
  }

  bool same(int x, int y) const { return find(x) == find(y); }

  int size(int x) const {
    int r = find(x);
    assert(data_[r]);
    int res = -*data_[r];
    assert(res > 0);
    return res;
  }
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
