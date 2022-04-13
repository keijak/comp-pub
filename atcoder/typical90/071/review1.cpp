#include <bits/stdc++.h>
#include <atcoder/scc>
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
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

optional<vector<int>> topological_sort(const vector<set<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u: g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.emplace_back(v);
    }
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u: g[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  if (int(res.size()) != n) {
    return nullopt;  // not a DAG
  }
  return res;
}

auto solve() {
  int n = in, m = in, K = in;
  vector<set<int>> g(n), rg(n);
  REP(i, m) {
    int a = int(in) - 1, b = int(in) - 1;
    g[b].insert(a);
    rg[a].insert(b);
  }
  auto topolo = topological_sort(rg);
  if (not topolo.has_value()) {
    print(-1);
    exit_();
  }
  set<vector<int>> perms;
  vector<int> used(n), visiting(n);
  set<int> nodeps;
  REP(i, n) {
    if (g[i].empty()) nodeps.insert(i);
  }

  auto f = Rec([&](auto &f, int v, vector<int> &p) -> void {
    if (ssize(perms) >= K) return;
    used[v] = true;
    p.push_back(v);
    if (ssize(p) == n) {
      perms.insert(p);
    }
    for (int u: rg[v]) {
      g[u].erase(v);
      if (g[u].empty()) nodeps.insert(u);
    }
    for (auto it = nodeps.begin(); it != nodeps.end(); ++it) {
      int x = *it;
      assert(not used[x]);
      nodeps.erase(x);
      f(x, p);
      if (ssize(perms) >= K) return;
      it = nodeps.insert(x).first;
    }
    p.pop_back();
    for (int u: rg[v]) {
      if (g[u].empty()) nodeps.erase(u);
      g[u].insert(v);
    }
    used[v] = false;
  });

  for (auto it = nodeps.begin(); it != nodeps.end(); ++it) {
    int x = *it;
    vector<int> p;
    nodeps.erase(it);
    f(x, p);
    if (ssize(perms) >= K) break;
    if (ssize(perms) == 0) break;
    it = nodeps.insert(x).first;
  }

  if (ssize(perms) < K) {
    print(-1);
    return;
  }
  assert(ssize(perms) == K);
  for (auto &p: perms) {
    REP(j, ssize(p)) {
      cout << p[j] + 1 << (j == ssize(p) - 1 ? '\n' : ' ');
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
