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
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
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
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

template<typename Monoid>
struct PersistentSegmentTree {
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
  i64 size_;
  NodePool *pool_;

  explicit PersistentSegmentTree(const std::vector<T> &v,
                                 NodePool *pool = NO_DELETE())
      : nil_(make_nil()), size_((i64) v.size()), pool_(pool) {
    root_ = build(v);
  }

  explicit PersistentSegmentTree(i64 n, NodePool *pool = NO_DELETE())
      : nil_(make_nil()), root_(nil_), size_(n), pool_(pool) {}

  PersistentSegmentTree set(i64 k, T x) const {
    NodePtr new_root = set_(k, std::move(x), root_, 0, size_);
    return {new_root, size_, pool_};
  }

  T fold(i64 kl, i64 kr) const { return fold_(kl, kr, root_, 0, size_); }
  T operator[](i64 k) const { return fold_(k, k + 1, root_, 0, size_); }

 private:
  PersistentSegmentTree(NodePtr root, i64 n, NodePool *pool)
      : nil_(make_nil()), root_(root), size_(n), pool_(pool) {}

  NodePtr build(const std::vector<T> &v) { return build(0, (i64) v.size(), v); }

  NodePtr build(i64 l, i64 r, const std::vector<T> &v) {
    if (l + 1 == r) return my_new(v[l]);
    i64 m = (l + r) >> 1;
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

  NodePtr set_(i64 k, T val, NodePtr n, i64 l, i64 r) const {
    if (l + 1 == r) return make_leaf(std::move(val));
    i64 m = (l + r) >> 1;
    if (k < m) return merge(set_(k, std::move(val), n->l, l, m), n->r);
    return merge(n->l, set_(k, std::move(val), n->r, m, r));
  }

  T fold_(i64 kl, i64 kr, NodePtr n, i64 l, i64 r) const {
    if (n == nil_) return Monoid::id();
    if (r <= kl or kr <= l) return Monoid::id();
    if (kl <= l and r <= kr) return n->data;
    i64 m = (l + r) >> 1;
    return Monoid::op(fold_(kl, kr, n->l, l, m), fold_(kl, kr, n->r, m, r));
  }

  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }
};

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, Q = in;
  vector<array<int, 3>> points(n);
  REP(i, n) {
    int x = in, y = in, w = in;
    points[i] = {x, y, w};
  }
  sort(ALL(points));

  vector<int> xs, ys;
  REP(i, n) {
    auto[x, y, w] = points[i];
    xs.push_back(x);
    ys.push_back(y);
  }
  Compressed<int> cy(ys);

  vector<PersistentSegmentTree<SumOp>> segs;
  segs.emplace_back(i64(n + 1));
  REP(i, n) {
    auto[x, y, w] = points[i];
    const auto &ps = segs[i];
    int yi = cy.index(y);
    segs.push_back(ps.set(yi, ps[yi] + i64(w)));
  }
  REP(i, Q) {
    int l = in, d = in, r = in, u = in;
    int li = lower_bound(ALL(xs), l) - xs.begin();
    int ri = lower_bound(ALL(xs), r) - xs.begin();
    int di = cy.index(d);
    int ui = cy.index(u);
    auto sr = segs[ri].fold(di, ui);
    auto sl = segs[li].fold(di, ui);
    print(sr - sl);
  }
}
