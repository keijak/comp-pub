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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "First" : "Second") << endc;
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


struct Diameter {
  int distance;
  std::vector<int> path;
};

// Tree diameter.
// Returns the maximum diameter and two endpoint nodes of the diameter.
Diameter tree_diameter(const std::vector<std::vector<int>> &g) {
  std::vector<int> parent(g.size());

  auto farthest = [&](auto &rec, int v, int p, int d) -> std::pair<int, int> {
    parent[v] = p;
    int rd = d, rv = v;
    for (auto u: g[v]) {
      if (u == p) continue;
      const auto &[child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > rd) {
        rd = child_d;
        rv = child_v;
      }
    }
    return {rd, rv};
  };

  const auto &[_, vr] = farthest(farthest, 0, -1, 0);
  const auto &[distance, vl] = farthest(farthest, vr, -1, 0);

  // Restore the path from vl to vr.
  std::vector<int> diameter_path;
  for (int v = vl; v != -1; v = parent[v]) {
    diameter_path.push_back(v);
  }
  assert(diameter_path.back() == vr);
  assert(int(diameter_path.size()) == distance + 1);

  return Diameter{distance, std::move(diameter_path)};
}

auto solve() {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  auto dia = tree_diameter(g);
  DUMP(dia.distance, dia.path);
  int d = dia.distance;
  if (d == 0) {
    return true;
  }
  if (d == 1) {
    return false;
  }
  return (d - 1) % 3 != 0;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    print(solve());
  }
}
