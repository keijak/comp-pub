#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

template<typename T = unsigned, int kBitWidth = std::numeric_limits<T>::digits>
struct PersistentBinaryTrie {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");

 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
    Node() : leaf_count(0), child{nullptr, nullptr} {}
  };
  NodePtr root_;  // The root node.

  struct NodePool {
    static constexpr size_t kInitBlockSize = 1u << 12;
    static constexpr double kBlockSizeGrowthRate = 1.5;  // Decrease if MLE.

    std::vector<std::unique_ptr<Node[]>> blocks_;
    size_t bsize_;
    size_t bi_;
    size_t ni_;

    NodePool() : bsize_(kInitBlockSize), bi_(0), ni_(0) {
      blocks_.emplace_back(new Node[kInitBlockSize]);
    }

    NodePtr new_node() {
      if (ni_ == bsize_) {
        bi_++;
        ni_ = 0;
        bsize_ = bsize_ * kBlockSizeGrowthRate;
        blocks_.emplace_back(new Node[bsize_]);
      }
      return &blocks_[bi_][ni_++];
    }
  };
  NodePool *pool_;

  PersistentBinaryTrie() : root_(nullptr), pool_(NO_DELETE()) {}
  explicit PersistentBinaryTrie(NodePool *p) : root_(nullptr), pool_(p) {}
  PersistentBinaryTrie(NodePtr r, NodePool *p) : root_(r), pool_(p) {}

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  PersistentBinaryTrie insert(T val) const {
    return PersistentBinaryTrie(insert_internal(root_, val), pool_);
  }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  PersistentBinaryTrie erase_one(T val) const {
    return PersistentBinaryTrie(erase_internal(root_, val), pool_);
  }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    assert(0 <= k and k < size());
    return get_internal(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    const int i = size() - k - 1;
    return (*this)[i];
  }

  // Returns the minimum index i s.t. trie[i] >= val.
  int lower_bound(T val) const { return count_less(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less(root_, val + 1); }

  // Counts the number of elements that are equal to `val`.
  // Note: BinaryTrie is a multiset.
  int count(T val) const {
    if (not root_) return 0;
    NodePtr t = root_;
    for (int i = kBitWidth - 1; i >= 0; i--) {
      t = t->child[val >> i & 1];
      if (not t) return 0;
    }
    return t->leaf_count;
  }

  std::vector<T> to_vec() const {
    std::vector<T> res;
    res.reserve(size());
    to_vec_internal(root_, T(0), res);
    return res;
  }

 private:
  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }

  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) const {
    NodePtr res = pool_->new_node();
    res->leaf_count = 1;
    if (t != nullptr) {
      res->leaf_count += t->leaf_count;
      res->child[0] = t->child[0];
      res->child[1] = t->child[1];
    }
    if (b < 0) return res;
    bool f = (val >> b) & 1;
    res->child[f] = insert_internal(res->child[f], val, b - 1);
    return res;
  }

  NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) const {
    assert(t);
    if (t->leaf_count == 1) {
      return nullptr;
    }
    NodePtr res = pool_->new_node();
    res->leaf_count = t->leaf_count - 1;
    res->child[0] = t->child[0];
    res->child[1] = t->child[1];
    if (b < 0) return res;
    bool f = (val >> b) & 1;
    res->child[f] = erase_internal(res->child[f], val, b - 1);
    return res;
  }

  T get_min(NodePtr t, T xor_mask, int b = kBitWidth - 1) const {
    assert(t != nullptr);
    if (b < 0) return 0;
    bool f = (xor_mask >> b) & 1;
    f ^= not t->child[f];
    return get_min(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  T get_internal(NodePtr t, int k, int b = kBitWidth - 1) const {
    if (b < 0) return 0;
    int m = t->child[0] ? t->child[0]->leaf_count : 0;
    return k < m ? get_internal(t->child[0], k, b - 1)
                 : get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  int count_less(NodePtr t, T val, int b = kBitWidth - 1) const {
    if (not t or b < 0) return 0;
    bool f = (val >> b) & 1;
    return (f and t->child[0] ? t->child[0]->leaf_count : 0) +
        count_less(t->child[f], val, b - 1);
  }

  void to_vec_internal(NodePtr t, T val, std::vector<T> &out,
                       int b = kBitWidth - 1) const {
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
};
using Trie = PersistentBinaryTrie<unsigned, 31>;

using namespace std;

auto solve() {
  INPUT(int, n, Q);
  INPUT(int, R, Rkey);
  unordered_map<int, unordered_set<int>> g;
  unordered_map<int, unsigned> keys;
  REP(i, n - 1) {
    INPUT(unsigned, u, v, k);
    g[u].insert(v);
    g[v].insert(u);
    keys[u] = k;
  }

  unordered_map<int, Trie> tries;
  auto dfs = [&](auto &rec, int v, int p, Trie trie) -> void {
    tries[v] = trie;
    for (auto u : g[v]) {
      if (u == p) continue;
      rec(rec, u, v, trie.insert(keys[u]));
    }
  };
  Trie t;
  dfs(dfs, R, -1, t.insert(Rkey));

  unsigned last_answer = 0;
  REP(q, Q) {
    INPUT(unsigned, qtype);
    qtype ^= last_answer;
    if (qtype == 0) {
      INPUT(unsigned, v, u, k);
      v ^= last_answer;
      u ^= last_answer;
      k ^= last_answer;
      tries[u] = tries[v].insert(k);
    } else {
      INPUT(unsigned, v, k);
      v ^= last_answer;
      k ^= last_answer;
      const Trie &tv = tries[v];
      auto a = tv.min_element(k);
      auto b = tv.max_element(k);
      a ^= k;
      b ^= k;
      print(a, b);
      last_answer = a ^ b;
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
