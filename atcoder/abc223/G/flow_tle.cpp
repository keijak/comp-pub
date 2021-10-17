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
  return (int) a.size();
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
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

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;
#include<atcoder/maxflow>

auto solve() {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u;
    --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> depth(n, -1);
  auto dfs = [&](auto &f, int v, int p, int d) -> void {
    depth[v] = d;
    for (auto u: g[v]) {
      if (u == p) continue;
      f(f, u, v, d + 1);
    }
  };
  dfs(dfs, 0, -1, 0);
  DUMP(depth);
  atcoder::mf_graph<int> mf(n + 2);
  int kSource = n;
  int kSink = n + 1;
  REP(v, n) {
    if (depth[v] % 2 == 0) {
      mf.add_edge(kSource, v, 1);
    } else {
      mf.add_edge(v, kSink, 1);
    }
  }
  map<pair<int, int>, int> edge_id;
  REP(v, n) {
    if (depth[v] % 2 == 0) {
      for (auto u: g[v]) {
        int eid = mf.add_edge(v, u, 1);
        edge_id[{v, u}] = edge_id[{u, v}] = eid;
      }
    }
  }

  auto maxflow = mf.flow(kSource, kSink);
  DUMP(maxflow);
  vector<int> ok(n, false);
  REP(v, n) {
    if (mf.get_edge(v).flow == 0) {
      ok[v] = true;
      continue;
    }
  }
  DUMP(ok);
  REP(v, n) {
    if (ok[v]) continue;

    check(mf.get_edge(v).flow == 1);

    int other = -1, oe = -1;
    for (auto u: g[v]) {
      int eid = edge_id[{v, u}];
      if (mf.get_edge(eid).flow == 1) {
        other = u;
        oe = eid;
        break;
      }
    }
    if (oe == -1) continue;
    check(mf.get_edge(other).flow == 1);

    mf.change_edge(v, 0, 0);
    mf.change_edge(oe, 1, 0);
    mf.change_edge(other, 1, 0);
    int fval = mf.flow(kSource, kSink);
    DUMP("try", v, other, fval);
    if (fval == 1) {
      DUMP("OK", v);
      ok[v] = true;
      mf.change_edge(v, 1, 0);
      mf.change_edge(oe, 1, 0);
      mf.change_edge(other, 1, 1);
    } else {
      mf.change_edge(v, 1, 1);
      mf.change_edge(oe, 1, 1);
      mf.change_edge(other, 1, 1);
    }
  }

  int ans = 0;
  REP(v, n) if (ok[v]) ++ans;
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
