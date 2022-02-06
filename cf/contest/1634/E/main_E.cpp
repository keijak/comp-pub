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

auto solve() -> optional<vector<string>> {
  int m = in;
  vector<vector<int>> as(m);
  vector<multiset<int>> g1(m);
  map<int, multiset<int>> g2;
  map<int, int> freq;
  REP(i, m) {
    int n = in;
    as[i].resize(n);
    REP(j, n) {
      int x = in;
      as[i][j] = x;
      g1[i].insert(x);
      g2[x].insert(i);
      ++freq[x];
    }
  }
  for (const auto&[k, v]: freq) {
    if (v % 2 == 1) return nullopt;
  }

  vector<map<int, int>> lcount(m);

  auto dfs = Rec([&](auto &dfs, int v, bool left) -> void {
    if (left) {
      if (g1[v].empty()) return;
      auto it = g1[v].begin();
      int c = *it;
      g1[v].erase(it);
      g2[c].erase(g2[c].find(v));
      lcount[v][c] += 1;
      dfs(c, false);
    } else {
      if (g2[v].empty()) return;
      auto it = g2[v].begin();
      int u = *it;
      g2[v].erase(it);
      g1[u].erase(g1[u].find(v));
      dfs(u, true);
    }
  });
  REP(v, m) {
    if (g1[v].empty()) continue;
    dfs(v, true);
  }
  vector<string> ans(m);
  REP(v, m) {
    ans[v].resize(ssize(as[v]));
    REP(j, ssize(as[v])) {
      int c = as[v][j];
      if (lcount[v][c]) {
        ans[v][j] = 'L';
        --lcount[v][c];
      } else {
        ans[v][j] = 'R';
      }
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    if (not ans) {
      print("NO");
    } else {
      print("YES");
      for (const auto &line: *ans) {
        print(line);
      }
    }
  }
}
