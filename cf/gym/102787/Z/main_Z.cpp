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

using namespace std;

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

template<class Monoids, size_t NODE_NUM = 1000000>
class GenericTreap {
 public:
  using T = typename Monoids::T;
  using F = typename Monoids::F;

  struct Node {
   public:
    T v, acc;
    int id;
    size_t pri;
    int size;
    Node *lch = nullptr, *rch = nullptr, *par = nullptr;

    Node() : v(Monoids::id()), acc(v), id(-1), pri(0), size(1) {}
    Node(T v, int id, size_t pri) : v(v), acc(v), id(id), pri(pri), size(1) {}
  };
  using NodePtr = struct Node *;
  NodePtr root;

  explicit GenericTreap(NodePtr p) : root(p) {}

  static NodePtr create_node(const T v, int id, size_t pri) {
    static std::unique_ptr<Node[]> pool(new Node[NODE_NUM]);
    static size_t i = 0;
    return &(pool[i++] = Node(v, id, pri));
  }

  static size_t random_priority() {
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0);
    return dist(engine);
  }

  static inline int count_(NodePtr p) { return p == nullptr ? 0 : p->size; }

  static inline T fold_(NodePtr p) {
    return p == nullptr ? Monoids::id() : p->acc;
  }

  static NodePtr pull_up_(NodePtr p) {
    p->size = count_(p->lch) + count_(p->rch) + 1;
    p->acc = p->v;
    if (p->lch) {
      p->acc = Monoids::op(p->lch->acc, p->v);
      p->lch->par = p;
    }
    if (p->rch) {
      p->acc = Monoids::op(p->acc, p->rch->acc);
      p->rch->par = p;
    }
    return p;
  }

  static NodePtr merge_(NodePtr l, NodePtr r) {
    if (l == nullptr || r == nullptr) {
      return l == nullptr ? r : l;
    }
    if (l->pri > r->pri) {
      l->rch = merge_(l->rch, r);
      l = pull_up_(l);
      l->par = nullptr;
      return l;
    } else {
      r->lch = merge_(l, r->lch);
      r = pull_up_(r);
      r->par = nullptr;
      return r;
    }
  }

  static std::pair<NodePtr, NodePtr> split_(NodePtr p, int k) {
    if (p == nullptr) return {nullptr, nullptr};

    if (k <= count_(p->lch)) {
      auto ret = split_(p->lch, k);
      p->lch = ret.second;
      p = pull_up_(p);
      p->par = nullptr;
      return {ret.first, p};
    } else {
      auto ret = split_(p->rch, k - count_(p->lch) - 1);
      p->rch = ret.first;
      p = pull_up_(p);
      p->par = nullptr;
      return {p, ret.second};
    }
  }

  void insert_(int k, T v, int id, size_t pri) {
    if (root == nullptr) {
      root = create_node(std::move(v), id, pri);
      return;
    }
    NodePtr l, r;
    std::tie(l, r) = split_(root, k);
    root = merge_(l, create_node(std::move(v), id, pri));
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

  static NodePtr get_root(NodePtr r) {
    while (r->par != nullptr) {
      r = r->par;
    }
    return r;
  }

  static GenericTreap merge(GenericTreap l, GenericTreap r) {
    return GenericTreap(merge_(l.root, r.root));
  }

  std::pair<GenericTreap, GenericTreap> split(int k) {
    auto ret = split_(root, k);
    return {GenericTreap(ret.first), GenericTreap(ret.second)};
  }

  void insert(int k, T v, int id) { insert_(k, std::move(v), id, random_priority()); }

  T erase(int k) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    root = merge_(l.root, r.root);
    return m.root->v;
  }

  T operator[](int k) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    T ret = m.root->v;
    root = merge_(merge_(l.root, m.root), r.root);
    return ret;
  }

  void set(int k, T v) {
    GenericTreap l, m, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, m) = l.split(k);
    m.root->v = std::move(v);
    m.root = pull_up_(m.root);
    root = merge_(merge_(l.root, m.root), r.root);
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

  T fold_all() const { return fold_(root); }

  std::vector<T> to_vec() {
    std::vector<T> v(size());
    to_vec_(root, v.begin());
    return v;
  }
};

struct AddSumOp {
  struct T {
    long long sum;
    int width;  // NOTE: Must be initialized with width=1!
  };
  using F = long long;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Add
  static T f_apply(const F &f, const T &x) {
    return {x.sum + f * x.width, x.width};
  }
  static F f_compose(const F &f, const F &g) { return f + g; }
  static constexpr F f_id() { return 0; }
};

using Treap = GenericTreap<AddSumOp>;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int Q = in;
  vector<Treap::NodePtr> treaps(Q, nullptr);

  REP(qi, Q) {
    int qt = in;
    if (qt == 1) {
      Int y = in;
      treaps[qi] = Treap::create_node({y, 1}, qi, Treap::random_priority());
    } else if (qt == 2) {
      int y = int(in) - 1, z = int(in) - 1;
      assert(treaps[y] != nullptr);
      assert(treaps[z] != nullptr);
      auto yp = Treap::get_root(treaps[y]);
      auto zp = Treap::get_root(treaps[z]);
      if (yp->id != zp->id) {
        auto merged = Treap::merge_(yp, zp);
        treaps[merged->id] = merged;
      }
    } else if (qt == 3) {
      int y = int(in) - 1, z = int(in);
      assert(treaps[y] != nullptr);
      auto yp = Treap::get_root(treaps[y]);
      if (yp->size > z) {
        auto[lp, rp] = Treap::split_(yp, z);
        if (lp != nullptr) treaps[lp->id] = lp;
        if (rp != nullptr) treaps[rp->id] = rp;
      }
    } else {
      int y = int(in) - 1;
      assert(treaps[y] != nullptr);
      auto yp = Treap::get_root(treaps[y]);
      print(Treap::fold_(yp).sum);
    }
  }
}
