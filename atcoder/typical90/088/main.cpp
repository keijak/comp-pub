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
inline int ssize(const T &a) {
  return (int) std::size(a);
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
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

using namespace std;

template<typename T = unsigned, int kBitWidth = 16>
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

  PersistentBinaryTrie() : root_(nullptr) {}
  explicit PersistentBinaryTrie(NodePtr r) : root_(r) {}

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  PersistentBinaryTrie insert(T val) const {
    return PersistentBinaryTrie(insert_internal(root_, val));
  }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  PersistentBinaryTrie erase_one(T val) const {
    return PersistentBinaryTrie(erase_internal(root_, val));
  }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    assert(0 <= k && k < size());
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
    if (!root_) return 0;
    NodePtr t = root_;
    for (int i = kBitWidth - 1; i >= 0; i--) {
      t = t->child[(val >> T(i)) & 1];
      if (!t) return 0;
    }
    return t->leaf_count;
  }

  vector<T> to_vec() const {
    vector<T> res;
    to_vec_internal(root_, 0, res);
    return res;
  }

 private:
  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) const {
    NodePtr res = new Node();
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
    NodePtr res = new Node();
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

  void to_vec_internal(NodePtr t, T val, vector<T> &out, int b = kBitWidth - 1) const {
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
using PTrie = PersistentBinaryTrie<>;

const int W = 8890;

auto solve() {
  INPUT(int, n, Q);
  vector<unsigned> a(n);
  cin >> a;
  vector<set<int>> ban(n);
  REP(q, Q) {
    INPUT(int, x, y);
    --x, --y;
    ban[y].insert(x);
  }
  auto dp = vector(n + 1, vector(W, optional<PTrie>()));
  const PTrie empty_trie;
  dp[0][0] = empty_trie;
  auto calc = [&]() -> pair<PTrie, PTrie> {
    REP(i, n) {
      unsigned x = a[i];
      REP(j, W) {
        if (not dp[i][j]) continue;
        const auto &trie = dp[i][j].value();
        if (dp[i + 1][j].has_value()) {
          return {trie, dp[i + 1][j].value()};
        }
        dp[i + 1][j] = trie;
        if (j + x >= W) continue;
        bool banned = false;
        for (auto p : ban[i]) {
          if (trie.count(p)) {
            banned = true;
            break;
          }
        }
        if (banned) continue;
        if (dp[i + 1][j + x].has_value()) {
          return {trie.insert(i), dp[i + 1][j + x].value()};
        }
        dp[i + 1][j + x] = trie.insert(i);
      }
    }
    assert(false);
  };
  auto[t1, t2] = calc();
  print(t1.size());
  auto v1 = t1.to_vec();
  for (auto &x : v1) { x++; }
  print_seq(v1);
  print(t2.size());
  auto v2 = t2.to_vec();
  for (auto &x : v2) { x++; }
  print_seq(v2);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    solve();
  }
}
