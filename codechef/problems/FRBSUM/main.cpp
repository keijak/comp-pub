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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

//template<typename T, typename = void>
//struct is_iterable : std::false_type {};
//template<typename T>
//struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
//                                  decltype(std::end(std::declval<T>()))>>
//    : std::true_type {
//};
//
//template<typename T, typename = std::enable_if_t<
//    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
//std::ostream &operator<<(std::ostream &os, const T &a) {
//  return print_seq(a, ", ", "", (os << "{")) << "}";
//}

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
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;
template<typename Monoid>
struct PersistentSegmentTree {
  using Int = long long;
  using T = typename Monoid::T;
  struct Node;
  using NodePtr = Node *;

  struct Node {
    T data;
    NodePtr l, r;
    Node() {}
    Node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
  };

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

  NodePtr nil_;
  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit PersistentSegmentTree(const std::vector<T> &v,
                                 NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((Int) v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentSegmentTree(Int n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentSegmentTree set(Int k, T x) const {
    NodePtr new_root = set_(k, std::move(x), root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(Int kl, Int kr) const { return fold_(kl, kr, root_, 0, size_); }
  T operator[](Int k) const { return fold_(k, k + 1, root_, 0, size_); }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t) min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

 private:
  PersistentSegmentTree(NodePtr root, Int n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) { return build(0, (Int) v.size(), v); }

  NodePtr build(Int l, Int r, const std::vector<T> &v) {
    if (l + 1 == r) return make_leaf(v[l]);
    Int m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  static NodePtr make_nil() {
    static Node nil_node(Monoid::id());
    nil_node.l = nil_node.r = &nil_node;
    return &nil_node;
  }

  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nil_;
    return p;
  }

  NodePtr merge(NodePtr l, NodePtr r) const {
    NodePtr p = pool_->new_node();
    p->data = Monoid::op(l->data, r->data);
    p->l = l;
    p->r = r;
    return p;
  }

  NodePtr set_(Int k, T val, NodePtr n, Int l, Int r) const {
    if (l + 1 == r) return make_leaf(std::move(val));
    Int m = (l + r) >> 1;
    if (k < m) return merge(set_(k, std::move(val), n->l, l, m), n->r);
    return merge(n->l, set_(k, std::move(val), n->r, m, r));
  }

  T fold_(Int kl, Int kr, NodePtr n, Int l, Int r) const {
    if (n == nil_) return Monoid::id();
    if (r <= kl or kr <= l) return Monoid::id();
    if (kl <= l and r <= kr) return n->data;
    Int m = (l + r) >> 1;
    return Monoid::op(fold_(kl, kr, n->l, l, m), fold_(kl, kr, n->r, m, r));
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

struct SumOp {
  using T = Int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  //static_assert(std::is_invocable_r_v<bool, F, T>);
  assert(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  assert(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

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

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in;
  const vector<Int> a = in(n);
  const Compressed<Int> ca(a);

  vector<PersistentSegmentTree<SumOp>> segs;
  segs.emplace_back(ca.size());
  REP(i, n) {
    const auto &seg = segs.back();
    int j = ca.index(a[i]);
    segs.push_back(seg.set(j, seg[j] + a[i]));
  }
  const int m = in;
  REP(i, m) {
    int l = in, r = in;
    --l;
    const auto &segl = segs[l];
    const auto &segr = segs[r];
    Int s = 0;
    while (true) {
      int r = ca.index(s + 2);
      Int t = segr.fold(0, r) - segl.fold(0, r);
      if (s == t) break;
      s = t;
    }
    print(s + 1);
  }
}
