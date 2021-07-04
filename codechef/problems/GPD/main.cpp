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

  PersistentBinaryTrie() : root_(nullptr) {}
  explicit PersistentBinaryTrie(NodePtr r) : root_(r) {}

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  PersistentBinaryTrie insert(T val) { return PersistentBinaryTrie(insert_internal(root_, val)); }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  PersistentBinaryTrie erase_one(T val) { return PersistentBinaryTrie(erase_internal(root_, val)); }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

 private:
  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) {
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

  NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) {
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
