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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
    static constexpr size_t kInitBlockSize = 1u << 14;
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
                  "predicate must be invocable on the value type");
    assert(0 <= r && r <= size_);
    const auto &fpos = min_left_false_(r, pred, Monoid::id(), root_, 0, size_).first;
    return fpos ? (*fpos + 1) : 0;
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
    return Monoid::op(fold_(kl, kr, np->l, nl, nm), fold_(kl, kr, np->r, nm, nr));
  }

  template<class F>
  std::pair<std::optional<Int>, typename Monoid::T> min_left_false_(
      Int kr, F pred, const typename Monoid::T &prval, NodePtr np, Int nl, Int nr) const {
    if (kr <= nl) return {std::nullopt, Monoid::id()};
    if (nr <= kr) {
      auto val = Monoid::op(np->data, prval);
      if (pred(val, nl)) { return {std::nullopt, std::move(val)}; }
      if (nl + 1 == nr) { return {nl, std::move(val)}; }
    }
    Int nm = (nl + nr) >> 1;
    auto rsub = min_left_false_(kr, pred, prval, np->r, nm, nr);
    if (rsub.first) { return rsub; }
    return min_left_false_(kr, pred, std::move(rsub.second), np->l, nl, nm);
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

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (true_x - false_x > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

auto solve() {
  int n = in;
  vector<int> a = in(n);
  vector<int> p(n, -1);
  {
    map<int, int> last;
    REP(i, n) {
      auto it = last.find(a[i]);
      if (it != last.end()) {
        p[i] = it->second;
      }
      last[a[i]] = i;
    }
  }
  vector<PersistentSegmentTree<SumOp>> freqs;
  freqs.emplace_back(n);
  REP(i, n) {
    auto s = freqs.back();
    if (p[i] != -1) {
      s = s.set(p[i], s[p[i]] + 1);
    }
    freqs.push_back(s);
  }
  vector<int> ans(n);
  for (int k = 1; k <= n; ++k) {
    int cnt = 1, r = n;
    while (cnt < n) {
      const auto &f = freqs[r];
      if (r - k <= f.fold_all()) {
        break;
      }
      r = f.min_left(r, [&](int x, int l) {
        return (r - l) - x <= k;
      });
      cnt++;
    }
    ans[k - 1] = cnt;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans);
  }
}
