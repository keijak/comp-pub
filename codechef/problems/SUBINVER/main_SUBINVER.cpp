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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

template<typename LazyMonoid>
struct PersistentLazySegmentTree {
  using T = typename LazyMonoid::T;
  using F = typename LazyMonoid::F;
  struct Node;
  using NodePtr = Node *;

  struct Node {
    T data;
    F lazy;
    NodePtr l, r;
    Node() {}
    Node(const T &data, const F &lazy) : data(data), lazy(lazy), l(nullptr), r(nullptr) {}
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

  explicit PersistentLazySegmentTree(const std::vector<T> &v,
                                     NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((Int) v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentLazySegmentTree(Int n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentLazySegmentTree set(Int k, const T &x) const {
    NodePtr new_root = set_(k, x, root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(Int kl, Int kr) const { return fold_(kl, kr, LazyMonoid::f_id(), root_, 0, size_); }
  T fold_all() const { return root_->data; }
  T operator[](Int k) const { return fold_(k, k + 1, LazyMonoid::f_id(), root_, 0, size_); }

  PersistentLazySegmentTree apply(Int kl, Int kr, const F &f) const {
    NodePtr new_root = apply_(kl, kr, f, root_, 0, size_);
    return {new_root, size_, pool_};
  }

 private:
  PersistentLazySegmentTree(NodePtr root, Int n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) {
    return build(0, (Int)
        v.size(), v);
  }

  NodePtr build(Int l, Int r, const std::vector<T> &v) {
    if (l + 1 == r) return make_leaf(v[l]);
    Int m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  static NodePtr make_nil() {
    static Node nil_node(LazyMonoid::id(), LazyMonoid::f_id());
    nil_node.l = nil_node.r = &nil_node;
    return &nil_node;
  }

  NodePtr make_leaf(T data) const {
    NodePtr p = pool_->new_node();
    p->data = std::move(data);
    p->lazy = LazyMonoid::f_id();
    p->l = p->r = nil_;
    return p;
  }

  NodePtr merge(NodePtr l, NodePtr r) const {
    NodePtr p = pool_->new_node();
    p->data = LazyMonoid::op(l->data, r->data);
    p->lazy = LazyMonoid::f_id();
    p->l = l;
    p->r = r;
    return p;
  }

  NodePtr set_(Int k, const T &val, NodePtr np, Int l, Int r) const {
    if (l + 1 == r) return make_leaf(val);
    NodePtr ltmp = apply_one(np->lazy, np->l);
    NodePtr rtmp = apply_one(np->lazy, np->r);
    Int m = (l + r) >> 1;
    if (k < m) {
      return merge(set_(k, val, ltmp, l, m), rtmp);
    } else {
      return merge(ltmp, set_(k, val, np->r, m, rtmp));
    }
  }

  T fold_(Int kl, Int kr, const F &f, NodePtr np, Int l, Int r) const {
    if (np == nil_) return LazyMonoid::id();
    if (r <= kl or kr <= l) return LazyMonoid::id();
    if (kl <= l and r <= kr) return LazyMonoid::f_apply(f, np->data);
    F f_down = LazyMonoid::f_compose(f, np->lazy);
    Int m = (l + r) >> 1;
    return LazyMonoid::op(fold_(kl, kr, f_down, np->l, l, m), fold_(kl, kr, f_down, np->r, m, r));
  }

  NodePtr apply_one(const F &f, NodePtr np) const {
    NodePtr p = pool_->new_node();
    p->data = LazyMonoid::f_apply(f, np->data);
    p->lazy = LazyMonoid::f_compose(f, np->lazy);
    p->l = np->l;
    p->r = np->r;
    return p;
  }

  NodePtr apply_(Int kl, Int kr, const F &f, NodePtr np, Int l, Int r) const {
    if (r <= kl or kr <= l) return np;
    if (l + 1 == r) {  // leaf
      return make_leaf(LazyMonoid::f_apply(f, np->data));
    }
    if (kl <= l and r <= kr) {
      return apply_one(f, np);
    }
    auto m = (l + r) >> 1;
    NodePtr l2 = apply_(kl, kr, f, apply_one(np->lazy, np->l), l, m);
    NodePtr r2 = apply_(kl, kr, f, apply_one(np->lazy, np->r), m, r);
    return merge(l2, r2);
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

struct XorRollingHashOp {
  using u64 = std::uint64_t;
  using u128 = __uint128_t;
  struct T {
    u64 hash;
    int width;
  };
  using F = char;
  static constexpr u64 kMod = (1ULL << 61) - 1;

  static T op(const T &x, const T &y) {
    u64 hash = mul(x.hash, pow_base(y.width));
    hash = add(hash, y.hash);
    return T{hash, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  static T f_apply(const F &f, const T &x) {
    if (f == 0) return x;
    return {add(add(pow_base(x.width), kMod - 1), kMod - x.hash), x.width};
  }
  static F f_compose(const F &f, const F &g) { return (f ^ g) & 1; }
  static constexpr F f_id() { return 0; }

 private:
  static u64 base() {
    static const u64 kBase = []() {
      std::random_device seed_gen;
      std::mt19937_64 engine(seed_gen());
      std::uniform_int_distribution<u64> rand(1, kMod - 1);
      return rand(engine);
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= kMod) ? (a - kMod) : a;
  }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & kMod);
    na += nb;
    return (na >= kMod) ? (na - kMod) : na;
  }

  static u64 pow_base(int i) {
    static vector<u64> kPowBase(1, 1);
    while (int(kPowBase.size()) <= i) {
      kPowBase.push_back(mul(kPowBase.back(), base()));
    }
    return kPowBase[i];
  }
};

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

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, U = in;
  vector<XorRollingHashOp::T> ini(n);
  REP(i, n) {
    ini[i] = {0, 1};
  }
  PersistentLazySegmentTree<XorRollingHashOp> seg(ini);
  auto best = seg;

  REP(qi, U) {
    int l = in, r = in;
    --l;
    seg = seg.apply(l, r, 1);
    if (seg.fold_all().hash == best.fold_all().hash) continue;
    int j = bisect<int>(n, 0, [&](int rx) {
      return seg.fold(0, rx).hash != best.fold(0, rx).hash;
    });
    if (best[j - 1].hash == 0) {
      best = seg;
    }
  }
  REP(i, n) {
    char v = (best[i].hash == 0) ? '0' : '1';
    cout << v;
  }
  cout << endl;
}
