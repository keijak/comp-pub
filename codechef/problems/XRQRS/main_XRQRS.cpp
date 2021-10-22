#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

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
  return (int) a.size();
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  return std::cout << x << endc;
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const std::string &sep = " ",
                        const std::string &ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
const backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

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
        bsize_ *= kBlockSizeGrowthRate;
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
  T max_element(NodePtr lp, T xor_mask = 0) const { return get_max(root_, lp, xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  //T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    check(0 <= k and k < size());
    return get_internal(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    const int i = size() - k - 1;
    return (*this)[i];
  }

  // Returns the minimum index i s.t. trie[i] >= val.
  //int lower_bound(T val) const { return count_less(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less(root_, val + 1); }

  int count_less2(NodePtr t, NodePtr lt, T val, int b = kBitWidth - 1) const {
    if (not t or b < 0) return 0;
    bool f = (val >> b) & 1;
    int cnt = 0;
    if (f and t->child[0]) {
      cnt += t->child[0]->leaf_count;
    }
    if (f and lt and lt->child[0]) {
      cnt -= lt->child[0]->leaf_count;
    }
    return cnt + count_less2(t->child[f], (lt == nullptr) ? nullptr : lt->child[f], val, b - 1);
  }

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
    check(t);
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

  T get_max(NodePtr t, NodePtr lt, T xor_mask, int b = kBitWidth - 1) const {
    check(t != nullptr);
    if (b < 0) return 0;
    bool f = not((xor_mask >> b) & 1);
    if (not t->child[f]
        or (lt and lt->child[f] and t->child[f]->leaf_count <= lt->child[f]->leaf_count)) {
      f = not f;
    }
    return get_max(t->child[f], (lt == nullptr) ? nullptr : lt->child[f], xor_mask, b - 1) | (T(f) << b);
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

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};
using Trie = PersistentBinaryTrie<>;

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  //static_assert(std::is_invocable_r_v<bool, F, T>);
  check(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  check(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

const int kBigX = 600000;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int Q = in;
  vector<Trie> a(1);
  REP(qi, Q) {
//    DUMP(qi, ssize(a));
//    REP(i, ssize(a)) {
//      DUMP(i, a[i].to_vec());
//    }

    int qt = in;
    if (qt == 0) {
      int x = in;
      a.push_back(a.back().insert(x));
    } else if (qt == 1) {
      int l = in, r = in, x = in;
      --l;
      Trie &lt = a.at(l);
      Trie &rt = a.at(r);
      auto res = rt.max_element(lt.root_, x);
      print(res);
    } else if (qt == 2) {
      int k = in;
      a.resize(ssize(a) - k);
    } else if (qt == 3) {
      int l = in, r = in, x = in;
      --l;
      Trie &lt = a.at(l);
      Trie &rt = a.at(r);
      int cnt = rt.count_less2(rt.root_, lt.root_, x + 1);
      print(cnt);
    } else {
      int l = in, r = in, k = in;
      --l;
      Trie &lt = a.at(l);
      Trie &rt = a.at(r);
      auto ans = bisect<int>(kBigX, -1, [&](int x) {
        int cnt = rt.count_less2(rt.root_, lt.root_, x + 1);
        return cnt >= k;
      });
      print(ans);
    }
  }
}
