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

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  vector<string> g(n);
  REP(i, n) {
    string s;
    cin >> g[i];
    assert(ssize(g[i]) == n);
  }
  vector<int> ans(m + 1);
  if (m == 1) {
    ans[0] = 1;
    ans[1] = 2;
    return ans;
  }

  vector<optional<int>> a_in(n), a_out(n), b_in(n), b_out(n);

  REP(i, n) REP(j, n) {
    if (i == j) continue;
    if (g[i][j] == g[j][i]) {
      REP(k, m + 1) { ans[k] = (k & 1) ? j + 1 : i + 1; }
      return ans;
    } else if (m & 1) {
      // odd
      REP(k, m + 1) { ans[k] = (k & 1) ? j + 1 : i + 1; }
      return ans;
    }

    if (g[i][j] == 'a') {
      a_in[j] = i;
      a_out[i] = j;
    } else if (g[i][j] == 'b') {
      b_in[j] = i;
      b_out[i] = j;
    }
  }

  auto gen = [&](int x, int y, int z) -> void {
    int mh = m / 2;
    if (mh & 1) {
      REP(i, mh) { ans[i] = (i & 1) ? y + 1 : x + 1; }
      REP(i, mh + 1) { ans[mh + i] = (i & 1) ? z + 1 : y + 1; }
    } else {
      REP(i, mh) { ans[i] = (i & 1) ? x + 1 : y + 1; }
      REP(i, mh + 1) { ans[mh + i] = (i & 1) ? z + 1 : y + 1; }
    }
  };

  REP(i, n) {
    if (a_in[i].has_value() and a_out[i].has_value()) {
      int x = a_in[i].value();
      int z = a_out[i].value();
      if (x != i and x != z and i != z) gen(x, i, z);
      return ans;
    }
    if (b_in[i].has_value() and b_out[i].has_value()) {
      int x = b_in[i].value();
      int z = b_out[i].value();
      if (x != i and x != z and i != z) gen(x, i, z);
      return ans;
    }
  }
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    auto res = solve();
    if (!res) {
      cout << "NO\n";
    } else {
      cout << "YES\n";
      print_seq(res.value());
    }
  }
}