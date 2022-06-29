// #define NDEBUG
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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
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

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u: g[v]) ++indeg[u];
  }
  MinHeap<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.push(v);
    }
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.top();
    q.pop();
    res.push_back(v);
    for (auto u: g[v]) {
      if (--indeg[u] == 0) {
        q.push(u);
      }
    }
  }
  if (int(res.size()) != n) {
    return nullopt;  // not a DAG
  }
  return res;
}

auto solve() -> bool {
  int n = in;
  vector<set<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].insert(b);
    g[b].insert(a);
  }
  string S = in;
  DUMP(S);

  vector<vector<int>> dg(n);
  {
    vector<int> color(n);
    REP(v, n) {
      color[v] = S[v] == 'B';
    }

    Rec([&](auto rec, int v, int p) -> bool {
      for (auto u: g[v]) {
        if (u == p) continue;
        bool black = rec(u, v);
        if (not black) {
          color[v] ^= 1;
        }
      }
      if (p == -1) {
        //
      } else if (color[v]) {
        dg[p].push_back(v);
      } else {
        dg[v].push_back(p);
      }

      return color[v];
    })(0, -1);
    DUMP(color);
    DUMP(dg);
    if (color[0]) return false;
  }
  auto res = topological_sort(dg);
  if (not res) return false;
  for (auto &e: *res) ++e;
  out_seq(*res);
  return true;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    if (not solve()) {
      out(-1);
    }
  }
  exit_();
}
