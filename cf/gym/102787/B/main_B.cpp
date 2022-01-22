#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using namespace std;

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

template<class ValueMonoid, class OperatorMonoid,
    class Modifier, std::size_t NODE_NUM = 600000>
class Treap {
 public:
  using value_structure = ValueMonoid;
  using value_type = typename value_structure::value_type;
  using operator_structure = OperatorMonoid;
  using operator_type = typename operator_structure::value_type;
  using modifier = Modifier;
  using size_type = std::size_t;

 private:
  class Node {
   public:
    using pointer = Node *;
    value_type v, acc;
    operator_type lazy;
    pointer lch, rch;
    size_type size, pri;

    Node() : v(value_structure::identity()), lazy(operator_structure::identity()),
             pri(0), size(1), acc(value_structure::identity()) {}

    Node(value_type v, size_type pri) :
        v(v), lazy(operator_structure::identity()), pri(pri), size(1), acc(v) {
      lch = rch = nullptr;
    }
  };

  static size_type xorshift() {
    static size_type x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    size_type t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
  }

 public:
//	using NodePtr = std::shared_ptr<Node>;
  using NodePtr = Node *;

 private:
  NodePtr root;

  static void apply(NodePtr p, const operator_type x) {
    p->lazy = operator_structure::operation(p->lazy, x);
    p->acc = modifier::operation(p->acc, p->lazy, count(p));
  }

 public:
  Treap() : root(nullptr) {}
  explicit Treap(NodePtr p) : root(p) {}

  static size_type count(NodePtr p) {
    return p == nullptr ? 0 : p->size;
  }
  static value_type query(NodePtr p) {
    return p == nullptr ? value_structure::identity() : p->acc;
  }

  static NodePtr eval(NodePtr p) {
    if (p->lazy != operator_structure::identity()) {
      p->v = modifier::operation(p->v, p->lazy, 1);
      if (p->lch != nullptr) {
        apply(p->lch, p->lazy);
      }
      if (p->rch != nullptr) {
        apply(p->rch, p->lazy);
      }
      p->lazy = operator_structure::identity();
    }
    return update(p);
  }

  static NodePtr update(NodePtr p) {
    p->size = count(p->lch) + count(p->rch) + 1;
    p->acc = value_structure::operation(
        value_structure::operation(query(p->lch), p->v),
        query(p->rch));
    return p;
  }

  static NodePtr merge(NodePtr l, NodePtr r) {
    if (l == nullptr || r == nullptr)
      return l == nullptr ? r : l;
    if (l->pri > r->pri) {
      l = eval(l);
      l->rch = merge(l->rch, r);
      return update(l);
    } else {
      r = eval(r);
      r->lch = merge(l, r->lch);
      return update(r);
    }
  }

  static std::pair<NodePtr, NodePtr> split(NodePtr p, size_type k) {
    if (p == nullptr) return std::make_pair(nullptr, nullptr);

    p = eval(p);
    if (k <= count(p->lch)) {
      std::pair<NodePtr, NodePtr> ret = split(p->lch, k);
      p->lch = ret.second;
      return std::make_pair(ret.first, update(p));
    } else {
      std::pair<NodePtr, NodePtr> ret = split(p->rch, k - count(p->lch) - 1);
      p->rch = ret.first;
      return std::make_pair(update(p), ret.second);
    }
  }

  static Treap merge(Treap l, Treap r) {
    return Treap(merge(l.root, r.root));
  }

  std::pair<Treap, Treap> split(size_type k) {
    std::pair<NodePtr, NodePtr> ret = split(root, k);
    return std::make_pair(Treap(ret.first), Treap(ret.second));
  }

  void insert(size_type k, const value_type v, size_type pri = xorshift()) {
    if (root == nullptr) {
      root = create_node(v, pri);
      return;
    }
    NodePtr l, r;
    std::tie(l, r) = split(root, k);
    root = merge(l, create_node(v, pri));
    root = merge(root, r);
  }

  value_type erase(size_type k) {
    Treap l, _, r;
    std::tie(l, r) = split(k + 1);
    std::tie(l, _) = l.split(k);
    root = merge(l.root, r.root);
    return _.root->v;
  }

  value_type kth_element(const size_type k) {
    Treap l, _, r;
    value_type ret;
    std::tie(l, r) = split(k);
    std::tie(l, _) = l.split(k - 1);
    ret = query(_.root);
    root = merge(l.root, merge(_.root, r.root));
    return ret;
  }

  value_type query(const size_type first, const size_type last) {
    Treap l, m, r;
    value_type ret;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    ret = query(m.root);
    root = merge(l.root, merge(m.root, r.root));
    return ret;
  }

  void update(const size_type first, const size_type last, const operator_type a) {
    Treap l, m, r;
    std::tie(l, r) = split(last);
    std::tie(l, m) = l.split(first);
    apply(m.root, a);
    root = merge(l.root, merge(m.root, r.root));
  }

  template<class InputIterator>
  void insert_from_iterator(InputIterator first, InputIterator last, size_type k = -1) {
    for (InputIterator itr = first; itr != last; ++itr) {
      if (k == -1) {
        insert(count(root), *itr);
      } else {
        insert(k++, *itr);
      }
    }
  }

  static NodePtr create_node(const value_type v, size_type pri) {
    static std::unique_ptr<Node[]> pool(new Node[NODE_NUM]);
    static size_t i = 0;
    return &(pool[i++] = Node(v, pri));
  }

  void dump(NodePtr p, typename std::vector<value_type>::iterator itr) {
    if (p == nullptr) return;
    p = eval(p);
    dump(p->lch, itr);
    *(itr + count(p->lch)) = p->v;
    dump(p->rch, itr + count(p->lch) + 1);
  }

  std::vector<value_type> dump(NodePtr p = nullptr) {
    if (p == nullptr) p = root;
    std::vector<value_type> v(count(p));
    dump(p, v.begin());
    return v;
  }
};

using u64 = std::uint64_t;
using RHash = std::tuple<u64, u64, int>; // LtoR, RtoL, width


struct RollingHash {
  using u128 = __uint128_t;
  static const u64 kMod = (1ULL << 61) - 1;

  static u64 base() {
    static const u64 kBase = []() {
      std::mt19937_64 engine(std::random_device{}());
      std::uniform_int_distribution<u64> rand(1000, kMod - 1);
      return rand(engine);
    }();
    return kBase;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= kMod) ? (a - kMod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, kMod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = u128(a) * b;
    u64 na = u64(t >> 61);
    u64 nb = u64(t & kMod);
    na += nb;
    return (na >= kMod) ? (na - kMod) : na;
  }

  static u64 pow_base(int i) {
    static std::vector<u64> kPowBase(1, 1);
    while (int(kPowBase.size()) <= i) {
      u64 val = mul(kPowBase.back(), base());
      kPowBase.push_back(val);
    }
    return kPowBase[i];
  }

  // Calculates hash(a || b) from hash(a), hash(b) and length(b).
  static u64 concat(u64 a_hash, u64 b_hash, int b_length) {
    return add(mul(a_hash, pow_base(b_length)), b_hash);
  }
};

class min_monoid {
 public:
  using value_type = RHash;
  static value_type identity() { return {0, 0, 0}; }
  static value_type operation(const value_type a, const value_type b) {
    const auto&[la, ra, wa] = a;
    const auto&[lb, rb, wb] = b;
    u64 lh = RollingHash::concat(la, lb, wb);
    u64 rh = RollingHash::concat(rb, ra, wa);
    return {lh, rh, wa + wb};
  }
};

class update_monoid {
 public:
  using value_type = RHash;
  static value_type identity() { return {0, 0, 0}; }
  static value_type operation(const value_type a, const value_type b) {
    if (b == identity())
      return a;
    return b;
  }
};

class modifier {
 public:
  using size_type = std::size_t;
  static min_monoid::value_type operation(min_monoid::value_type a,
                                          update_monoid::value_type b, size_type sz) {
    if (b == update_monoid::identity())
      return a;
    return b;
  }
};

using Seq = Treap<min_monoid, update_monoid, modifier>;
auto solve() {
  int n;
  cin >> n;
  Seq seq;
  REP(i, n) {
    u64 h = i + 1;
    seq.insert(i, {h, h, 1});
  }

  auto shuf = [&](int a, int b) {
    if (a >= b) return;
    if (b - a < n - b) {
      int w = b - a;
      auto[tmp, rr] = seq.split(b + w);
      auto[tmp2, bt] = tmp.split(b);
      auto[ll, at] = tmp2.split(a);
      seq = Seq::merge(Seq::merge(ll, bt), Seq::merge(at, rr));
    } else {
      int w = n - b;
      auto[tmp, bt] = seq.split(b);
      auto[tmp2, mid] = tmp.split(a + w);
      auto[ll, at] = tmp2.split(a);
      seq = Seq::merge(Seq::merge(ll, bt), Seq::merge(mid, at));
    }
  };

  REP(i, n) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    shuf(a, b);
  }
  return seq.dump();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, Q;
  cin >> n >> Q;
  string S;
  cin >> S;
  Seq seq;
  REP(i, n) {
    u64 h = S[i];
    seq.insert(i, {h, h, 1});
  }
  REP(qi, Q) {
    int qt;
    cin >> qt;
    if (qt == 1) {
      int l, r;
      cin >> l >> r;
      --l;
      auto[mt, rt] = seq.split(r);
      auto[lt, _] = mt.split(l);
      seq = Seq::merge(lt, rt);

    } else if (qt == 2) {
      char c;
      int p;
      cin >> c >> p;
      --p;
      seq.insert(p, {u64(c), u64(c), 1});
    } else {
      int l, r;
      cin >> l >> r;
      --l;
      auto[lh, rh, w] = seq.query(l, r);
      cout << (lh == rh ? "yes" : "no") << '\n';
    }

    // Debug
//    auto deb = seq.dump();
//    cerr << "Q[" << qi << "]: ";
//    for (auto x: deb) {
//      cerr << char(get<0>(x));
//    }
//    cerr << endl;
  }
}
