#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

struct Edge {
  int to;
  i64 len;
};

i64 solve() {
  int n;
  cin >> n;
  vector<i64> w(n);
  cin >> w;
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u, v, c;
    cin >> u >> v >> c;
    --u, --v;
    g[u].push_back({v, c});
    g[v].push_back({u, c});
  }

  vector<vector<pair<i64, int>>> best_from(n);

  auto pre = [&](auto &pre, int v, int p) -> i64 {
    vector<pair<i64, int>> bf;
    for (auto &e : g[v]) {
      if (e.to == p) continue;
      i64 bfr = pre(pre, e.to, v);
      if (bfr > e.len) {
        bf.push_back({bfr - e.len, e.to});
        sort(ALL(bf), greater{});
        while (ssize(bf) > 2) {
          bf.pop_back();
        }
      }
    }
    best_from[v] = bf;
    i64 ret = w[v];
    if (not bf.empty()) {
      ret += bf[0].first;
    }
    return ret;
  };
  pre(pre, 0, -1);
  REP(i, n) { DEBUG(i, best_from[i]); }

  vector<map<i64, i64>> best_to(n);
  auto dfs_to = [&](auto &rec, int v, int p) -> const map<i64, i64> & {
    map<i64, i64> mp;
    mp[0] = w[v];
    for (auto &e : g[v]) {
      if (e.to == p) continue;
      const auto &sub = rec(rec, e.to, v);
      for (auto &[sub_cost, sub_gain] : sub) {
        i64 gain = w[v] - e.len + sub_gain;
        if (gain <= 0) continue;
        i64 cost;
        if (w[v] > e.len + sub_cost) {
          cost = 0;
        } else {
          cost = e.len + sub_cost - w[v];
        }
        auto it = mp.upper_bound(cost);
        while (it != mp.end() and it->second <= gain) {
          it = mp.erase(it);
        }
        if (it != mp.begin()) {
          --it;
          if (it->second >= gain) continue;
        }
        mp[cost] = gain;
      }
    }
    best_to[v] = move(mp);
    return best_to[v];
  };
  dfs_to(dfs_to, 0, -1);

  auto dfs = [&](auto &dfs, int v, int p) -> i64 {
    const vector<pair<i64, int>> bf = best_from[v];
    i64 best = w[v];
    for (auto &e : g[v]) {
      if (e.to == p) continue;
      i64 start = w[v];
      if (not bf.empty() and bf[0].second != e.to) {
        start += bf[0].first;
      } else if (ssize(bf) > 1 and bf[1].second != e.to) {
        start += bf[1].first;
      }
      if (e.len > start) continue;
      const auto &mp = best_to[e.to];
      auto it = mp.upper_bound(start - e.len);
      if (it == mp.begin()) continue;
      --it;
      i64 sub = (start - e.len) + it->second;
      chmax(best, sub);
    }
    for (auto &e : g[v]) {
      if (e.to == p) continue;
      i64 sub = dfs(dfs, e.to, v);
      chmax(best, sub);
    }
    return best;
  };
  return dfs(dfs, 0, -1);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
