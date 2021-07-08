#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

template<typename T = unsigned, int kBitWidth = std::numeric_limits<T>::digits>
struct BinaryTrie {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");

 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
    mutable T lazy_mask;
    Node() : leaf_count(0), child{nullptr, nullptr}, lazy_mask(0) {}
  };
  NodePtr root_;  // The root node.

  BinaryTrie() : root_(nullptr) {}

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
    assert(0 <= k && k < size());
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
  void push_down(NodePtr t, int b) const {
    if (t->lazy_mask == 0) return;
    if ((t->lazy_mask >> b) & 1) std::swap(t->child[0], t->child[1]);
    if (t->child[0]) t->child[0]->lazy_mask ^= t->lazy_mask;
    if (t->child[1]) t->child[1]->lazy_mask ^= t->lazy_mask;
    t->lazy_mask = 0;
  }

  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    if (!t) t = new Node();
    t->leaf_count += 1;
    if (b < 0) return t;
    push_down(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = insert_internal(t->child[f], val, b - 1);
    return t;
  }

  NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) {
    assert(t);
    t->leaf_count -= 1;
    if (t->leaf_count == 0) {
      delete t;
      return nullptr;
    }
    if (b < 0) return t;
    push_down(t, b);
    bool f = (val >> b) & 1;
    t->child[f] = erase_internal(t->child[f], val, b - 1);
    return t;
  }

  T get_min(NodePtr t, T xor_mask, int b = kBitWidth - 1) const {
    assert(t);
    if (b < 0) return 0;
    push_down(t, b);
    bool f = (xor_mask >> b) & 1;
    f ^= !t->child[f];
    return get_min(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  T get_internal(NodePtr t, int k, int b = kBitWidth - 1) const {
    if (b < 0) return 0;
    push_down(t, b);
    int m = t->child[0] ? t->child[0]->leaf_count : 0;
    return k < m ? get_internal(t->child[0], k, b - 1)
                 : get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  int count_less(NodePtr t, T val, int b = kBitWidth - 1) const {
    if (!t || b < 0) return 0;
    push_down(t, b);
    bool f = (val >> b) & 1;
    return (f && t->child[0] ? t->child[0]->leaf_count : 0) +
        count_less(t->child[f], val, b - 1);
  }
};

using namespace std;

auto solve() -> optional<vector<u64>> {
  INPUT(int, n);
  vector<u64> b(n);
  BinaryTrie<u64> trie;
  REP(i, n) {
    cin >> b[i];
    trie.insert(b[i]);
  }
  vector<u64> ans(n);
  u64 pb = *min_element(ALL(b));
  ans[0] = pb;
  DUMP(pb);
  trie.erase_one(pb);
  u64 acc = 0;
  u64 pa = pb;
  REP(i, 1, n) {
    trie.xor_all(pb);
    acc ^= pb;
    int j = trie.upper_bound(pa);
    if (j == trie.size()) return nullopt;
    auto ca = trie[j];
    assert(ca > pa);
    ans[i] = ca ^ acc;
    DUMP(i, ca, ans[i]);
    trie.erase_one(ca);
    pa = ca;
    pb = ans[i];
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print("No");
    } else {
      print("Yes");
      print_seq(*ans);
    }
  }
}