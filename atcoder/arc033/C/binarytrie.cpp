#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

template<typename T = uint32_t, int kBitWidth = std::numeric_limits<T>::digits>
struct BinaryTrie {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");
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

  BinaryTrie() = default;
  explicit BinaryTrie(NodePtr p) : root_(p) {}
  BinaryTrie(std::initializer_list<T> vals) {
    for (auto val: vals) { this->insert(val); }
  }

  int size() const { return size(root_); }

  bool empty() const { return size() == 0; }

  void insert(T val) { root_ = insert_internal(root_, val); }

  // Removes one element that is eaual to `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  void erase_one(T val) { root_ = erase_internal(root_, val); }

  // Returns k-th smallest value (0-indexed).
  T operator[](int k) const {
    assert(0 <= k and k < size());
    return get_internal(root_, k);
  }

  // Splits the trie into two: k smallest items and (n-k) largest items.
  std::pair<BinaryTrie, BinaryTrie> split(int k) {
    auto[t1, t2] = split_internal(root_, k);
    return {BinaryTrie(t1), BinaryTrie(t2)};
  }

  // Merges two tries into this. Destructs the other.
  void merge(BinaryTrie other) {
    root_ = merge_internal(root_, other.root_);
  }

  std::vector<T> to_vec() const {
    std::vector<T> ret;
    to_vec_internal(root_, T(0), ret);
    return ret;
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

  static NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    assert(t);
    t->leaf_count -= 1;
    if (t->leaf_count == 0) return nullptr;
    if (b < 0) return t;
    bool f = (val >> b) & 1;
    t->child[f] = erase_internal(t->child[f], val, b - 1);
    return t;
  }

  static T get_internal(NodePtr t, int k, int b = kBitWidth - 1) {
    if (b < 0) return 0;
    int m = size(t->child[0]);
    if (k < m) {
      return get_internal(t->child[0], k, b - 1);
    } else {
      return get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
    }
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

  static std::pair<NodePtr, NodePtr> split_internal(NodePtr t, int pos, int b = kBitWidth - 1) {
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

  static NodePtr merge_internal(NodePtr t1, NodePtr t2) {
    if (not t2) return t1;
    if (not t1) return t2;
    t1->leaf_count = size(t1) + size(t2);
    t1->child[0] = merge_internal(t1->child[0], t2->child[0]);
    t1->child[1] = merge_internal(t1->child[1], t2->child[1]);
    return t1;
  }
};
using Trie = BinaryTrie<uint32_t, 18>;

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int Q = in;
  Trie trie;
  REP(i, Q) {
    int T = in, X = in;
    if (T == 1) {
      trie.insert(X);
    } else {
      auto val = trie[X - 1];
      trie.erase_one(val);
      print(val);
    }
  }
}
