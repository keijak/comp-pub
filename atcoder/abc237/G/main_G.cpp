#include <bits/stdc++.h>
using namespace std;

template<typename T = uint32_t, int kBitWidth = std::numeric_limits<T>::digits>
struct BinaryTrie {
 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
    Node() : leaf_count(0), child{nullptr, nullptr} {}
    explicit Node(int n, NodePtr l = nullptr, NodePtr r = nullptr) : leaf_count(n), child{l, r} {}
  };
  NodePtr root_ = nullptr;
  bool rev = false;

  BinaryTrie() = default;
  explicit BinaryTrie(NodePtr p) : root_(p) {}
  BinaryTrie(std::initializer_list<T> vals) {
    for (auto val: vals) { this->insert(val); }
  }

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  void insert(T val) { root_ = insert_internal(root_, val); }

  pair<BinaryTrie, BinaryTrie> split(int pos) {
    if (rev) {
      pos = size() - pos;
    }
    auto[t1, t2] = split_internal(root_, pos);
    BinaryTrie bt1(t1), bt2(t2);
    if (rev) {
      bt1.rev = bt2.rev = true;
      return {bt2, bt1};
    }
    return {bt1, bt2};
  }

  static BinaryTrie merge(BinaryTrie t1, BinaryTrie t2) {
    return BinaryTrie(merge_internal(t1.root_, t2.root_));
  }

  void append_to(std::vector<T> &out) const {
    to_vec_internal(root_, T(0), out);
    if (this->rev) {
      std::reverse(out.end() - size(), out.end());
    }
  }

 private:
  static NodePtr make_node(int leaf_count, NodePtr l = nullptr, NodePtr r = nullptr) {
    static std::deque<Node> pool;
    pool.emplace_back(leaf_count, l, r);
    return &pool.back();
  }

  static int size(NodePtr t) {
    return (t == nullptr) ? 0 : t->leaf_count;
  }

  static NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    if (not t) t = make_node(0);
    t->leaf_count += 1;
    if (b < 0) return t;
    bool f = (val >> b) & 1;
    t->child[f] = insert_internal(t->child[f], val, b - 1);
    return t;
  }

  static void to_vec_internal(NodePtr t, T val, std::vector<T> &out,
                              int b = kBitWidth - 1) {
    if (not t) return;
    if (b < 0) {
      out.push_back(val);
      return;
    }
    if (t->child[0]) {
      to_vec_internal(t->child[0], val, out, b - 1);
    }
    if (t->child[1]) {
      to_vec_internal(t->child[1], val | (T(1) << b), out, b - 1);
    }
  }

  static pair<NodePtr, NodePtr> split_internal(NodePtr t, int pos, int b = kBitWidth - 1) {
    if (not t) return {nullptr, nullptr};
    if (b < 0) {
      if (pos <= 0) return {nullptr, t};
      else return {t, nullptr};
    }
    const int lsize = size(t->child[0]);
    if (lsize <= pos) {
      auto[lz, rz] = split_internal(t->child[1], pos - lsize, b - 1);
      return {make_node(size(t->child[0]) + size(lz), t->child[0], lz),
              make_node(size(rz), nullptr, rz)};
    } else {
      auto[lz, rz] = split_internal(t->child[0], pos, b - 1);
      return {make_node(size(lz), lz, nullptr),
              make_node(size(rz) + size(t->child[1]), rz, t->child[1])};
    }
  }

  static NodePtr merge_internal(NodePtr t1, NodePtr t2, int b = kBitWidth - 1) {
    if (not t2) return t1;
    if (not t1) {
      swap(t1, t2);
      return t1;
    }
    const int msize = size(t1) + size(t2);
    NodePtr lc = nullptr, rc = nullptr;
    if (b >= 0) {
      lc = merge_internal(t1->child[0], t2->child[0], b - 1);
      rc = merge_internal(t1->child[1], t2->child[1], b - 1);
    }
    t1->child[0] = lc;
    t1->child[1] = rc;
    t1->leaf_count = msize;
    return t1;
  }
};
using Trie = BinaryTrie<uint32_t, 18>;

auto solve() {
  int n, Q, X;
  cin >> n >> Q >> X;
  --X;

  vector<uint32_t> P(n);
  for (auto &x: P) cin >> x, --x;

  map<int, Trie> tries;
  for (int i = 0; i < n; ++i) tries[i].insert(P[i]);

  auto do_sort = [&](int L, int R, bool rev) {
    auto lit = tries.upper_bound(L);
    {
      int j = (--lit)->first;
      if (j != L) {
        auto[t0, t1] = lit->second.split(L - j);
        tries[j] = t0;
        lit = tries.emplace(L, t1).first;
      }
    }
    auto rit = tries.upper_bound(R);
    {
      int j = (--rit)->first;
      if (j != R) {
        auto[t0, t1] = rit->second.split(R - j);
        tries[j] = t0;
        rit = tries.emplace(R, t1).first;
      }
    }
    Trie merged;
    for (auto it = lit; it != rit; it = tries.erase(it)) {
      merged = Trie::merge(merged, it->second);
    }
    merged.rev = rev;
    if (not merged.empty()) {
      tries[L] = merged;
    }
  };

  for (int i = 0; i < Q; ++i) {
    int C, L, R;
    cin >> C >> L >> R;
    do_sort(L - 1, R, C == 2);
  }

  vector<uint32_t> result;
  for (const auto&[_, t]: tries) t.append_to(result);
  auto it = std::find(result.begin(), result.end(), X);
  return 1 + int(it - result.begin());
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << endl;
}
