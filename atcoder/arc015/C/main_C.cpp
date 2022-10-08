// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

using Uint = unsigned long long;
//using Real = long double;
#include <boost/multiprecision/cpp_dec_float.hpp>
//using Real = boost::multiprecision::cpp_dec_float_100;
using Real = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<400>>;

#include <boost/multiprecision/cpp_int.hpp>
namespace multip = boost::multiprecision;
using Rat = multip::cpp_rational;
using Int = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

using Factors = map<Int, int>;

Factors factorize(Int n) {
  assert(n > 0);
  Factors res;
  for (Int k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    int count = 0;
    do {
      n /= k;
      ++count;
    } while (n % k == 0);
    res[k] = count;
  }
  if (n > 1) {
    res[n] = 1;
  }
  return res;
}

struct Node;

struct Edge {
  Int scale;
  Node *to;
};

struct Node {
  int id;
  string name;
  vector<Edge> child;
  vector<Edge> parent;
  pair<Int, string> repr;

  Node(int i, const string &s) : id(i), name{s}, child{}, repr{1, s} {}
};

auto solve() {
  const int N = in;
  map<string, int> node_id;
  map<int, Factors> factors_table;
  vector<std::unique_ptr<Node>> nodes;
  REP(i, N) {
    string L, R;
    Int m;
    cin >> L >> m >> R;
    if (not node_id.count(L)) {
      int id = ssize(nodes);
      nodes.push_back(make_unique<Node>(id, L));
      node_id[L] = id;
    }
    Node *lp = nodes[node_id.at(L)].get();
    if (not node_id.count(R)) {
      int id = ssize(nodes);
      nodes.push_back(make_unique<Node>(id, R));
      node_id[R] = id;
    }
    Node *rp = nodes[node_id.at(R)].get();
    factors_table[m] = factorize(m);
    lp->child.push_back(Edge{m, rp});
    rp->parent.push_back(Edge{m, lp});
  }

  const int n = ssize(nodes);

  auto search = [&](const int &unit) -> pair<Int, int> {
    queue<pair<Factors, int>> q;
    vector<optional<Factors>> cost_table(n);
    cost_table[unit] = Factors{};
    q.emplace(Factors{}, unit);

    auto push = [&](const int v, Factors cost) -> bool {
      if (not cost_table[v].has_value()) {
        cost_table[v] = cost;
        q.emplace(cost, v);
        return true;
      }
      assert(cost_table[v] == cost);
      return false;
    };

    while (not q.empty()) {
      auto [cost, v] = q.front();
      q.pop();
      Node *node = nodes.at(v).get();
      for (const auto &e: node->child) {
        if (cost_table[e.to->id].has_value()) continue;
        Factors f = cost;
        for (auto [p, k]: factors_table.at(e.scale)) {
          f[p] -= k;
          if (f[p] == 0) f.erase(p);
        }
        push(e.to->id, f);
      }
      for (const auto &e: node->parent) {
        if (cost_table[e.to->id].has_value()) continue;
        Factors f = cost;
        for (auto [p, k]: factors_table.at(e.scale)) {
          f[p] += k;
          if (f[p] == 0) f.erase(p);
        }
        push(e.to->id, f);
      }
    }

    Int max_cost = -1;
    int max_id = -1;
    REP(v, n) {
      if (not cost_table[v].has_value()) continue;
      const Factors &f = cost_table[v].value();
      Int val = 1;
      for (auto [p, k]: f) {
        if (k < 0) {
          val = -1;
          break;
        }
        REP(j, k) val *= p;
      }
      if (chmax(max_cost, val)) {
        max_id = v;
      }
    }
    return pair{max_cost, max_id};
  };

  string x, y;
  Int ans = -1;
  REP(id, nodes.size()) {
    Node *node = nodes[id].get();
    auto res = search(id);
    if (chmax(ans, res.first)) {
      x = nodes[res.second]->name;
      y = node->name;
    }
  }
  cout << "1" << x << "=" << ans << y << endl;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
