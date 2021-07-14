#include <cassert>
#include <iostream>
#include <vector>

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
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector g(n, vector(n, true));
  REP(i, n) g[i][i] = false;
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a][b] = false;
    g[b][a] = false;
  }

  vector<int> scan_done(n, 0);

  auto dfs_bip = [&](auto self, int v, int p, int &cnt_a, int &cnt_b,
                     bool is_a) -> bool {
    REP(u, n) {
      if (u == p) continue;
      if (not g[u][v]) continue;
      if (scan_done[u] == scan_done[v]) return false;
      if (scan_done[u] != 0) continue;
      if (is_a) {
        scan_done[u] = -1;
        ++cnt_b;
      } else {
        scan_done[u] = +1;
        ++cnt_a;
      }
      if (not self(self, u, v, cnt_a, cnt_b, not is_a)) return false;
    }
    return true;
  };

  vector<int> deltas;
  REP(i, n) {
    if (scan_done[i]) continue;
    scan_done[i] = 1;
    int cnt_a = 1, cnt_b = 0;
    bool ok = dfs_bip(dfs_bip, i, -1, cnt_a, cnt_b, true);
    if (not ok) return -1;
    DEBUG(cnt_a, cnt_b);
    int delta = abs(cnt_a - cnt_b);
    deltas.push_back(delta);
  }
  DEBUG(scan_done);
  DEBUG(deltas);
  int dsum = 0;
  for (auto d : deltas) dsum += d;
  int target = dsum / 2;

  const int D = ssize(deltas);
  vector<bool> sums(target + 1);
  sums[0] = true;
  REP(i, D) {
    int d = deltas[i];
    for (int j = target; j >= 0; --j) {
      if (j + d <= target and sums[j]) {
        sums[j + d] = true;
      }
    }
  }
  int delta_a = target;
  while (not sums[delta_a]) delta_a--;
  int delta_b = dsum - delta_a;
  int pd = abs(delta_a - delta_b);
  i64 size_a = (n - pd) / 2;
  i64 size_b = n - size_a;
  DEBUG(size_a, size_b);
  return size_a * (size_a - 1) / 2 + size_b * (size_b - 1) / 2;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
