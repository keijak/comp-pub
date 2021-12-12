#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
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

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
#define cerr if(false)cerr
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
    static constexpr size_t kInitialBlockSize = 1u << 12;
    static constexpr double kBlockSizeGrowthRate = 1.5;  // Try smaller rate if MLE.

    std::vector<std::unique_ptr<Node[]>> blocks_;
    size_t bsize_;
    size_t bi_;
    size_t ni_;

    NodePool() : bsize_(kInitialBlockSize), bi_(0), ni_(0) {
      blocks_.emplace_back(new Node[kInitialBlockSize]);
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
  T fold_all() const { return root_->data; }
  T operator[](Int k) const { return fold_(k, k + 1, root_, 0, size_); }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t) min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

  template<class F>
  Int min_left(Int r, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, typename Monoid::T, int>,
                  "predicate must be invocable on (folded, left index).");
    assert(0 <= r && r <= size_);
    const auto &fpos = min_left_(r, pred, Monoid::id(), root_, 0, size_).first;
    return fpos.value_or(-1) + 1;
  }

  template<class F>
  Int max_right(Int l, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, typename Monoid::T, int>,
                  "predicate must be invocable on (folded, right index).");
    assert(0 <= l && l < size_);
    const auto &fpos = max_right_(l, pred, Monoid::id(), root_, 0, size_).first;
    return fpos.value_or(size_ + 1) - 1;
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

  NodePtr set_(Int k, T val, NodePtr np, Int nl, Int nr) const {
    if (nl + 1 == nr) return make_leaf(std::move(val));
    Int nm = (nl + nr) >> 1;
    if (k < nm) return merge(set_(k, std::move(val), np->l, nl, nm), np->r);
    return merge(np->l, set_(k, std::move(val), np->r, nm, nr));
  }

  T fold_(Int kl, Int kr, NodePtr np, Int nl, Int nr) const {
    if (np == nil_) return Monoid::id();
    if (nr <= kl or kr <= nl) return Monoid::id();
    if (kl <= nl and nr <= kr) return np->data;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_(kl, kr, np->l, nl, nm),
                      fold_(kl, kr, np->r, nm, nr));
  }

  template<class F>
  std::pair<std::optional<Int>, typename Monoid::T> min_left_(
      Int kr, F pred, const typename Monoid::T &prval, NodePtr np, Int nl,
      Int nr) const {
    if (kr <= nl) return {std::nullopt, Monoid::id()};
    if (nr <= kr) {
      auto val = Monoid::op(np->data, prval);
      if (pred(val, nl)) {
        return {std::nullopt, std::move(val)};
      }
      if (nl + 1 == nr) {
        return {nl, std::move(val)};
      }
    }
    const Int nm = (nl + nr) >> 1;
    const auto rsub = min_left_(kr, pred, prval, np->r, nm, nr);
    if (rsub.first) {
      return rsub;
    }
    return min_left_(kr, pred, std::move(rsub.second), np->l, nl, nm);
  }

  template<class F>
  std::pair<std::optional<Int>, typename Monoid::T> max_right_(
      Int kl, F pred, const typename Monoid::T &plval, NodePtr np, Int nl,
      Int nr) const {
    if (nr <= kl) return {std::nullopt, Monoid::id()};
    if (kl <= nl) {
      auto val = Monoid::op(plval, np->data);
      if (pred(val, nl)) {
        return {std::nullopt, std::move(val)};
      }
      if (nl + 1 == nr) {
        return {nl, std::move(val)};
      }
    }
    const Int nm = (nl + nr) >> 1;
    const auto lsub = max_right_(kl, pred, plval, np->l, nl, nm);
    if (lsub.first) {
      return lsub;
    }
    return max_right_(kl, pred, std::move(lsub.second), np->r, nm, nr);
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

struct SumOp {
  using T = int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, Q = in;
  vector<pair<int, int>> xy(n);
  REP(i, n) {
    int x = in, y = in;
    xy[i] = {x, y};
  }
  sort(ALL(xy));
  vector<int> xs(n);
  vector<PersistentSegmentTree<SumOp>> segs;
  segs.reserve(n + 1);
  segs.emplace_back(n + 1);
  REP(i, n) {
    auto[x, y] = xy[i];
    xs[i] = x;
    auto &p = segs.back();
    segs.push_back(p.set(y, p[y] + 1));
  }
  REP(i, Q) {
    int l1 = in, l2 = in, r1 = in, r2 = in;
    int j1 = lower_bound(ALL(xs), l1) - xs.begin();
    int j2 = upper_bound(ALL(xs), l2) - xs.begin();
    int hi = segs[j2].fold(r1, r2 + 1);
    int lo = segs[j1].fold(r1, r2 + 1);
    print(hi - lo);
  }
}