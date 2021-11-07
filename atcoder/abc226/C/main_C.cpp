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

optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
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
  int n = in;
  vector<int> tim(n);
  vector<vector<int>> g(n);
  REP(i, n) {
    int t = in;
    tim[i] = t;
    int k = in;
    vector<int> a = in(k);
    for (auto &e: a) {
      --e;
      g[i].push_back(e);
    }
  }
  vector<int> nes(n, 0);
  auto dfs = [&](auto &f, int v) -> void {
    for (auto u: g[v]) {
      if (nes[u]) continue;
      nes[u] = 1;
      f(f, u);
    }
  };
  nes[n - 1] = 1;
  dfs(dfs, n - 1);
  Int ans = 0;
  REP(v, n) {
    if (nes[v]) ans += tim[v];
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
