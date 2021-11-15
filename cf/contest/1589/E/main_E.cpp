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

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
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

Int kOffset;

template<typename Monoid>
struct DualSegmentTree {
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

  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit DualSegmentTree(Int n, NodePool *pool = NO_DELETE()) : size_(n), pool_(pool) {
    root_ = make_leaf(Monoid::id());
  }

  void apply(Int kl, Int kr, T x) { apply_(kl, kr, x, root_, 0, size_); }
  T operator[](Int k) const { return get_(k, root_, 0, size_); }

  std::vector<T> to_vec(Int size = -1) const {
    if (size < 0) size = size_;
    std::vector<T> res((size_t) min(size, size_));
    for (Int i = 0; i < size; ++i) {
      res[i] = (*this)[i];
    }
    return res;
  }

 private:
  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nullptr;
    return p;
  }

  void apply_(Int kl, Int kr, const T &val, NodePtr np, Int nl, Int nr) {
    if (np == nullptr) return;
    if (nr <= kl or kr <= nl) return;
    if (kl <= nl and nr <= kr) {
      np->data = Monoid::op(np->data, val);
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (np->l == nullptr) np->l = make_leaf(Monoid::id());
    apply_(kl, kr, val, np->l, nl, nm);
    if (np->r == nullptr) np->r = make_leaf(Monoid::id());
    apply_(kl, kr, val, np->r, nm, nr);
  }

  T get_(Int k, NodePtr np, Int nl, Int nr) const {
    if (np == nullptr) return Monoid::id();
    if (nl + 1 == nr) return np->data;
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      return Monoid::op(get_(k, np->l, nl, nm), np->data);
    } else {
      return Monoid::op(get_(k, np->r, nm, nr), np->data);
    }
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

// Keeps the value with the latest time.
struct AssignOp {
  // Value with a timestamp.
  struct T {
    int time;
    long long value;
  };
  static T op(const T &x, const T &y) { return (x.time > y.time) ? x : y; }
  static T id() { return {0, 0}; }
  friend ostream &operator<<(ostream &os, const T &val) {
    return os << "{" << val.time << ": " << val.value << "}";
  }
};

void add_value(DualSegmentTree<AssignOp> &seg, Int k, int time, Int delta) {
  auto pval = seg[k];
  seg.apply(k, k + 1, {time, pval.value + delta});
}

const Int kMax = 8e14;
using SegTree = DualSegmentTree<AssignOp>;

auto solve() {
  kOffset = 4e14;
  int n = in;
  vector<Int> a = in(n);
  SegTree::NodePool pool;
  SegTree seg(kMax, &pool);
  Int ans = 0;
  REP(i, n) {
    Int x = a[i];
    if (i % 2 == 0) {
      kOffset -= x;
      seg.apply(0, kOffset, {2 * (i + 1), 0});
    } else {
      kOffset += x;
      seg.apply(kOffset + 1, kMax, {2 * (i + 1), 0});
    }
    Int delta = (i % 2 == 0) ? x : -x;
    add_value(seg, kOffset + delta, 2 * (i + 1) + 1, 1);
    ans += seg[kOffset].value;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
