#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

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

bool solve() {
  // n <= 2^18 (=262,144)
  int n, m;
  cin >> n >> m;
  int p = 0;
  while ((1LL << p) != n) ++p;

  vector<u32> a(m);
  cin >> a;
  vector<bool> bset(n, true);
  for (auto x : a) {
    bset[x] = false;
  }
  bset[0] = false;
  vector<u32> b;
  REP(x, n) if (bset[x]) b.push_back(x);

  vector<optional<pair<u32, int>>> basis(p);
  int basis_count = 0;
  REP(j, ssize(b)) {
    u32 x = b[j];
    REP(i, p) {
      if (~x & (1 << i)) continue;
      if (basis[i].has_value()) {
        x ^= basis[i]->first;
      } else {
        basis[i] = pair(x, j);
        ++basis_count;
        break;
      }
    }
  }
  if (basis_count != p) return false;

  set<u32> nodes = {0};
  vector<pair<int, int>> edges;
  auto dfs = [&](auto &dfs, int i, u32 v) {
    if (i == p) return;
    u32 x = b[basis[i]->second];
    u32 u = v ^ x;
    if (not nodes.count(u)) {
      nodes.insert(u);
      edges.emplace_back(v, u);
      dfs(dfs, i + 1, u);
    }
    dfs(dfs, i + 1, v);
  };
  dfs(dfs, 0, 0);
  DEBUG(ssize(edges));
  if (ssize(edges) != n - 1) return false;
  for (auto [u, v] : edges) {
    cout << u << " " << v << "\n";
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  bool ok = solve();
  if (!ok) {
    cout << "-1\n";
  }
}
