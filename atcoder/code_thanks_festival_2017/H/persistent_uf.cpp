#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)

template <typename T, int K = 20>
struct PersistentArray {
  struct Node;
  // No memory release by default.
  using NodePtr = Node *;  // ALT: std::shared_ptr<Node>

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

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<PersistentUnionFind> ufs(m + 1);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    ufs[i + 1] = ufs[i].unite(a, b);
  }

  auto united_time = [&](int x, int y) -> int {
    if (not ufs[m].same(x, y)) return -1;
    int fv = 0, tv = m;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      if (ufs[mid].same(x, y)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  };

  int q;
  cin >> q;
  REP(i, q) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    cout << united_time(x, y) << "\n";
  }
}
