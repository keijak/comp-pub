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

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

static size_t xorshift() {
  static size_t x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  size_t t = (x ^ (x << 11));
  x = y;
  y = z;
  z = w;
  return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

template<class Monoids, size_t NODE_NUM = 1000000>
class GenericTreap {
 private:
  using T = typename Monoids::T;
  using F = typename Monoids::F;

  struct Node {
   public:
    T v, acc;
    F lazy;
    size_t pri;
    int size;
    Node *lch = nullptr, *rch = nullptr;

    Node() : v(Monoids::id()), acc(v), lazy(Monoids::f_id()), pri(0), size(1) {}
    Node(T v, size_t pri) : v(v), acc(v), lazy(Monoids::f_id()), pri(pri), size(1) {}
  };
  using NodePtr = struct Node *;
  NodePtr root;

  explicit GenericTreap(NodePtr p) : root(p) {}

  static NodePtr create_node(const T v, size_t pri) {
    static std::unique_ptr<Node[]> pool(new Node[NODE_NUM]);
    static size_t i = 0;
    return &(pool[i++] = Node(v, pri));
  }

  static size_t random_priority() {
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0);
    return dist(engine);
  }

  static int count_(NodePtr p) { return p == nullptr ? 0 : p->size; }

  static T fold_(NodePtr p) { return p == nullptr ? Monoids::id() : p->acc; }

  static void apply_(NodePtr p, const F &val) {
    p->v = Monoids::f_apply(val, p->v);
    p->acc = Monoids::f_apply(val, p->acc);
    p->lazy = Monoids::f_compose(val, p->lazy);
  }

  static NodePtr push_down_(NodePtr p) {
    if (p->lazy != Monoids::f_id()) {
      if (p->lch != nullptr) apply_(p->lch, p->lazy);
      if (p->rch != nullptr) apply_(p->rch, p->lazy);
      p->lazy = Monoids::f_id();
    }
    return p;
  }

  static NodePtr pull_up_(NodePtr p) {
    p->size = count_(p->lch) + count_(p->rch) + 1;
    p->acc = p->lch ? Monoids::op(p->lch->acc, p->v) : p->v;
    if (p->rch) p->acc = Monoids::op(p->acc, p->rch->acc);
    return p;
  }

  static NodePtr merge_(NodePtr l, NodePtr r) {
    if (l == nullptr || r == nullptr) {
      return l == nullptr ? r : l;
    }
    if (l->pri > r->pri) {
      l = push_down_(l);
      l->rch = merge_(l->rch, r);
      return pull_up_(l);
    } else {
      r = push_down_(r);
      r->lch = merge_(l, r->lch);
      return pull_up_(r);
    }
  }

  static std::pair<NodePtr, NodePtr> split_(NodePtr p, int k) {
    if (p == nullptr) return {nullptr, nullptr};

    p = push_down_(p);
    if (k <= count_(p->lch)) {
      auto ret = split_(p->lch, k);
      p->lch = ret.second;
      return {ret.first, pull_up_(p)};
    } else {
      auto ret = split_(p->rch, k - count_(p->lch) - 1);
      p->rch = ret.first;
      return {pull_up_(p), ret.second};
    }
  }

  void insert_(int k, T v, size_t pri) {
    if (root == nullptr) {
      root = create_node(v, pri);
      return;
    }
    NodePtr l, r;
    std::tie(l, r) = split_(root, k);
    root = merge_(l, create_node(std::move(v), pri));
    root = merge_(root, r);
  }

  void to_vec_(NodePtr p, typename std::vector<T>::iterator itr) {
    if (p == nullptr) return;
    to_vec_(p->lch, itr);
    *(itr + count_(p->lch)) = p->v;
    to_vec_(p->rch, itr + count_(p->lch) + 1);
  }

 public:
  GenericTreap() : root(nullptr) {}

  int size() { return count_(root); }

  static GenericTreap merge(GenericTreap l, GenericTreap r) { return GenericTreap(merge_(l.root, r.root)); }

  std::pair<GenericTreap, GenericTreap> split(int k) {
    auto ret = split_(root, k);
    return {GenericTreap(ret.first), GenericTreap(ret.second)};
  }

  void insert(int k, T v) { insert_(k, std::move(v), random_priority()); }

  T erase(int k) {
    GenericTreap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    root = merge_(l.root, r.root);
    return _.root->v;
  }

  T operator[](int k) {
    GenericTreap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    T ret = _.root->v;
    root = merge_(merge_(l.root, _.root), r.root);
    return ret;
  }

  void set(int k, T v) {
    GenericTreap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    _.root->v = std::move(v);
    root = merge_(merge_(l.root, _.root), r.root);
  }

  std::vector<T> to_vec(NodePtr p = nullptr) {
    if (p == nullptr) p = root;
    std::vector<T> v(count_(p));
    to_vec_(p, v.begin());
    return v;
  }
  T fold(int first, int last) {
    GenericTreap l, m, r;
    T ret;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    ret = fold_(m.root);
    root = merge_(merge_(l.root, m.root), r.root);
    return ret;
  }

  void apply(int first, int last, const F &val) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    apply_(m.root, val);
    root = merge_(merge_(l.root, m.root), r.root);
  }
};

using namespace std;

// Range Flip Range Sum over 0-1 sequence.
struct XorSumOp {
  struct T {
    int ones;
    int width;  // NOTE: Must be initialized with width=1!
    int max_ones;
    int max_zeros;
    int l_ones;
    int l_zeros;
    int r_ones;
    int r_zeros;
  };
  using F = int;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    int l_ones = x.l_ones;
    if (x.l_ones == x.width) l_ones += y.l_ones;
    int l_zeros = x.l_zeros;
    if (x.l_zeros == x.width) l_zeros += y.l_zeros;

    int r_ones = y.r_ones;
    if (y.r_ones == y.width) r_ones += x.r_ones;
    int r_zeros = y.r_zeros;
    if (y.r_zeros == y.width) r_zeros += x.r_zeros;

    return {
        x.ones + y.ones, x.width + y.width,
        max({x.max_ones, y.max_ones, x.r_ones + y.l_ones}),
        max({x.max_zeros, y.max_zeros, x.r_zeros + y.l_zeros}),
        l_ones, l_zeros, r_ones, r_zeros,
    };
  }
  static T id() { return {0, 0, 0, 0, 0, 0, 0, 0}; }

  // Update: 0-1 flip by f=1.
  static T f_apply(const F &f, const T &x) {
    if (f == 0) return x;
    return {x.width - x.ones, x.width, x.max_zeros, x.max_ones,
            x.l_zeros, x.l_ones, x.r_zeros, x.r_ones};
  }
  static F f_compose(const F &f, const F &g) { return f ^ g; }
  static constexpr F f_id() { return 0; }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, Q = in;
  string S = in;

  using Treap = GenericTreap<XorSumOp>;
  Treap tree;

  //LazySegmentTree<XorSumOp> tree(n);

  REP(i, n) {
    int one = S[i] == '1';
    tree.insert(i, {
        one, 1,
        one, 1 - one,
        one, 1 - one,
        one, 1 - one
    });
  }
  REP(qi, Q) {
    int qt = in, l = in, r = in;
    --l;
    assert(qt == 1);

//    vector<int> deb(n);
//    REP(i, n) deb[i] = tree.fold(i, i + 1).max_zeros;
//    DUMP(qi, deb);

    tree.apply(l, r, 1);
    auto res = tree.fold(0, n);
    cout << max(res.max_ones, res.max_zeros) << '\n';
  }
}
