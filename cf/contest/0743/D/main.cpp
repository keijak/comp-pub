#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
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

optional<i64> solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  auto is_path = [&]() -> bool {
    if (ssize(g[0]) > 1) return false;
    for (int i = 1; i < n; ++i) {
      if (ssize(g[i]) > 2) return false;
    }
    return true;
  }();
  if (is_path) return nullopt;

  vector<i64> subsum(n);
  auto dfs_ss = [&](auto self, int v, int p) -> i64 {
    i64 res = a[v];
    for (auto u : g[v]) {
      if (u == p) continue;
      res += self(self, u, v);
    }
    return subsum[v] = res;
  };
  dfs_ss(dfs_ss, 0, -1);

  auto dfs_top2 = [&](auto self, int v, int p) -> pair<i64, optional<i64>> {
    i64 top1 = subsum[v];
    optional<i64> top2;
    vector<i64> top2v;
    for (auto u : g[v]) {
      if (u == p) continue;
      auto [t1, t2] = self(self, u, v);
      chmax(top1, t1);
      top2v.push_back(t1);
      if (t2.has_value()) {
        if (not top2.has_value()) {
          top2 = t2.value();
        } else {
          top2 = max(top2.value(), t2.value());
        }
      }
    }
    sort(ALL(top2v), greater<>());
    while (ssize(top2v) > 2) top2v.pop_back();
    if (ssize(top2v) == 2) {
      i64 x = accumulate(ALL(top2v), 0LL);
      if (not top2.has_value()) {
        top2 = x;
      } else if (top2.value() < x) {
        top2 = x;
      }
    }
    return {top1, top2};
  };
  auto [t1, t2] = dfs_top2(dfs_top2, 0, -1);
  return t2;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (not res) {
    cout << "Impossible\n";
  } else {
    cout << *res << "\n";
  }
}
