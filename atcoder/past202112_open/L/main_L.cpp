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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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
struct SegmentTree {
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

  NodePtr root_;
  Int size_;
  NodePool *pool_;

  explicit SegmentTree(Int n, NodePool *pool = NO_DELETE())
      : size_(n), pool_(pool) {
    root_ = make_node(Monoid::id());
  }

  void set(Int k, T x) { set_(k, std::move(x), root_, 0, size_); }

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

 private:
  NodePtr make_node(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->l = p->r = nullptr;
    return p;
  }

  void set_(Int k, T val, NodePtr np, Int nl, Int nr) {
    if (nl + 1 == nr) {
      np->data = std::move(val);
      return;
    }
    Int nm = (nl + nr) >> 1;
    if (k < nm) {
      if (np->l == nullptr) np->l = make_node(Monoid::id());
      set_(k, std::move(val), np->l, nl, nm);
    } else {
      if (np->r == nullptr) np->r = make_node(Monoid::id());
      set_(k, std::move(val), np->r, nm, nr);
    }
    np->data = Monoid::op(np->l == nullptr ? Monoid::id() : np->l->data,
                          np->r == nullptr ? Monoid::id() : np->r->data);
  }

  T fold_(Int kl, Int kr, NodePtr np, Int nl, Int nr) const {
    if (np == nullptr) return Monoid::id();
    if (nr <= kl or kr <= nl) return Monoid::id();
    if (kl <= nl and nr <= kr) return np->data;
    Int nm = (nl + nr) >> 1;
    return Monoid::op(fold_(kl, kr, np->l, nl, nm),
                      fold_(kl, kr, np->r, nm, nr));
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

struct MaxOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -kBigVal<int>; }
};

auto solve() {
  Int n = in, P = in;
  ++P;
  vector<Int> a = in(n);
  for (auto &x: a) ++x;
  reverse(ALL(a));
  //assert(P <= 10000000);

  //auto dp = vector(n + 1, vector(P + 100, -kBigVal<int>));
  SegmentTree<MaxOp> dp(P + 100);
  dp.set(0, 0);

  //dp[0][0] = 0;
  Int offset = 0;
  REP(i, n) {
    //DUMP(i, dp[i]);
    ++offset;

    // i-th person is a liar.
    //dp[i + 1] = dp[i];

    // i-th person is honest.
    if (a[i] - offset >= 0) {
      int maxval = dp.fold(0, a[i] - offset + 1) + 1;
      if (dp[a[i] - offset] < maxval) {
        dp.set(a[i] - offset, maxval);
      }
    }

//    for (Int j = 0; j < a[i] - (offset - 1); ++j) {
//      if (dp[i][j] == -kBigVal<int>) continue;
//      if (a[i] - offset >= 0) chmax(dp[i + 1][a[i] - offset], dp[i][j] + 1);
//    }
  }
  //Int ans = n - *max_element(ALL(dp[n]));
  Int ans = n - dp.fold(0, P - n + 1);
  print(clamp(ans, 0LL, n));
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}
