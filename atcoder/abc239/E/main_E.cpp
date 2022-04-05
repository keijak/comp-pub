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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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
 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
#ifdef ENABLE_BINARY_TRIE_LAZY_MASK
    mutable T lazy_mask = 0;
#endif
    Node() : leaf_count(0), child{nullptr, nullptr} {}
    explicit Node(int n, NodePtr l = nullptr, NodePtr r = nullptr)
        : leaf_count(n), child{l, r} {}
  };
  NodePtr root_ = nullptr;

  BinaryTrie() = default;
  explicit BinaryTrie(NodePtr p) : root_(p) {}
  BinaryTrie(std::initializer_list<T> vals) {
    for (auto val: vals) {
      this->insert(val);
    }
  }

  int size() const { return size_(root_); }

  bool empty() const { return size() == 0; }

  // Converts to a vector.
  std::vector<T> to_vec() const {
    std::vector<T> res;
    to_vec_(root_, T(0), res);
    return res;
  }

  // Adds `val`.
  void insert(T val) { root_ = insert_(root_, val); }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  void erase_one(T val) { root_ = erase_(root_, val); }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min_(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min_(root_, xor_mask); }

  // Counts the number of elements that are equal to `val`.
  // Note: BinaryTrie is a multiset.
  int count(T val) const { return count_(root_, val); }

  // Returns the minimum index i s.t. trie[i] >= val.
  int lower_bound(T val) const { return count_less_(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less_(root_, val + 1); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    assert(0 <= k and k < size());
    return get_(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    assert(0 <= k and k < size());
    return get_(root_, size() - k - 1);
  }

  // Splits the trie into two: k smallest items and (n-k) largest items.
  std::pair<BinaryTrie, BinaryTrie> split(int pos) {
    auto[t1, t2] = split_(root_, pos);
    return {BinaryTrie(t1), BinaryTrie(t2)};
  }

  // Merges the other trie into this and destructs the other.
  void merge(BinaryTrie other) { root_ = merge_(root_, other.root_); }

 private:
  static NodePtr make_node_(int leaf_count, NodePtr l = nullptr,
                            NodePtr r = nullptr) {
    static std::deque<Node> pool;
    pool.emplace_back(leaf_count, l, r);
    return &pool.back();
  }

  // Propagates the lazy state to the children.
  static inline void push_down_(NodePtr t, int b) {
#ifdef ENABLE_BINARY_TRIE_LAZY_MASK
    if (t->lazy_mask == 0) return;
    if ((t->lazy_mask >> b) & 1) std::swap(t->child[0], t->child[1]);
    if (t->child[0]) t->child[0]->lazy_mask ^= t->lazy_mask;
    if (t->child[1]) t->child[1]->lazy_mask ^= t->lazy_mask;
    t->lazy_mask = 0;
#endif
  }

  static int size_(NodePtr t) { return (t == nullptr) ? 0 : t->leaf_count; }

  static void to_vec_(NodePtr t, T val, std::vector<T> &out,
                      int b = kBitWidth - 1) {
    if (t == nullptr) return;
    if (b < 0) {
      out.push_back(val);
      return;
    }
    push_down_(t, b);
    if (t->child[0]) {
      to_vec_(t->child[0], val, out, b - 1);
    }
    if (t->child[1]) {
      to_vec_(t->child[1], val | (T(1) << b), out, b - 1);
    }
  }

  static NodePtr insert_(NodePtr t, T val, int b = kBitWidth - 1) {
    if (t == nullptr) t = make_node_(0);
    t->leaf_count += 1;
    if (b < 0) return t;
    push_down_(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = insert_(t->child[f], val, b - 1);
    return t;
  }

  static NodePtr erase_(NodePtr t, T val, int b = kBitWidth - 1) {
    t->leaf_count -= 1;
    if (t->leaf_count == 0) return nullptr;
    if (b < 0) return t;
    push_down_(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = erase_(t->child[f], val, b - 1);
    return t;
  }

  static T get_(NodePtr t, int k, int b = kBitWidth - 1) {
    if (b < 0) return 0;
    push_down_(t, b);
    int m = size_(t->child[0]);
    return k < m ? get_(t->child[0], k, b - 1)
                 : get_(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  static T get_min_(NodePtr t, T xor_mask, int b = kBitWidth - 1) {
    if (b < 0) return 0;
    push_down_(t, b);
    bool f = (xor_mask >> b) & 1;
    f ^= not t->child[f];
    return get_min_(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  static int count_(NodePtr t, T val, int b = kBitWidth - 1) {
    if (t == nullptr) return 0;
    if (b < 0) return t->leaf_count;
    push_down_(t, b);
    bool f = (val >> b) & 1;
    return count_(t->child[f], val, b - 1);
  }

  static int count_less_(NodePtr t, T val, int b = kBitWidth - 1) {
    if (not t or b < 0) return 0;
    push_down_(t, b);
    bool f = (val >> b) & 1;
    return (f ? size_(t->child[0]) : 0) + count_less_(t->child[f], val, b - 1);
  }

  static std::pair<NodePtr, NodePtr> split_(NodePtr t, int pos,
                                            int b = kBitWidth - 1) {
    if (t == nullptr) return {nullptr, nullptr};
    if (b < 0) {
      if (pos <= 0) {
        return {nullptr, t};
      } else {
        return {t, nullptr};
      }
    }
    const int lsize = size_(t->child[0]);
    if (lsize <= pos) {
      auto[lz, rz] = split_(t->child[1], pos - lsize, b - 1);
      return {make_node_(size_(t->child[0]) + size_(lz), t->child[0], lz),
              make_node_(size_(rz), nullptr, rz)};
    } else {
      auto[lz, rz] = split_(t->child[0], pos, b - 1);
      return {make_node_(size_(lz), lz, nullptr),
              make_node_(size_(rz) + size_(t->child[1]), rz, t->child[1])};
    }
  }

  static NodePtr merge_(NodePtr t1, NodePtr t2, int b = kBitWidth - 1) {
    if (t2 == nullptr) return t1;
    if (t1 == nullptr) return t2;
    push_down_(t1, b);
    push_down_(t2, b);
    t1->leaf_count = size_(t1) + size_(t2);
    t1->child[0] = merge_(t1->child[0], t2->child[0], b - 1);
    t1->child[1] = merge_(t1->child[1], t2->child[1], b - 1);
    return t1;
  }
};
using IntSet = BinaryTrie<uint32_t, 30>;

using namespace std;

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

auto solve() {
  int n = in, Q = in;
  vector<int> X = in(n);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    g[a - 1].push_back(b - 1);
    g[b - 1].push_back(a - 1);
  }
  vector<vector<pair<int, int>>> queries(n);
  REP(i, Q) {
    int v = in, k = in;
    queries[v - 1].push_back({k - 1, i});
  }

  vector<int> answer(Q);

  auto res = Rec([&](auto &f, int v, int p) -> IntSet {
    IntSet s;
    s.insert(X[v]);
    for (auto u: g[v]) {
      if (u == p) continue;
      s.merge(f(u, v));
    }
    for (auto[k, q]: queries[v]) {
      answer[q] = s.kth_largest(k);
    }
    return s;
  })(0, -1);
  DUMP(res.to_vec());
  print_seq(answer, "\n");
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
