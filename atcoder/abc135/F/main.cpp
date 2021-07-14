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

// Z-algorithm
// Returns the Z array of the given string.
// Z[i] := the longest common prefix length between S[0:] and S[i:].
vector<int> ZArray(string_view s) {
  int n = s.size();
  vector<int> Z(n);
  Z[0] = n;
  int i = 1, j = 0;
  while (i < n) {
    while (i + j < n and s[j] == s[i + j]) ++j;
    Z[i] = j;
    if (j == 0) {
      ++i;
      continue;
    }
    int k = 1;
    for (; k < j and k + Z[k] < j; ++k) {
      Z[i + k] = Z[k];
    }
    i += k;
    j -= k;
  }
  return Z;
}

const int BIG = 1e9;

i64 solve() {
  string s, t;
  cin >> s >> t;
  const int ns = ssize(s), nt = ssize(t);
  string u = t;
  while (ssize(u) <= nt + ns + nt) {
    u += s;
  }
  DEBUG(u);
  auto z = ZArray(u);
  DEBUG(z);

  deque<bool> sub(ns);
  for (int i = nt; i < nt + ns; ++i) {
    int lcp = z[i];
    sub[i - nt] = lcp >= nt;
  }
  DEBUG(sub);
  vector<optional<int>> hops(ns);
  deque<bool> searching(ns);
  auto f = [&](auto &f, int i) -> int {
    if (hops[i].has_value()) return *hops[i];
    if (not sub[i]) return 0;
    if (searching[i]) return BIG;
    searching[i] = true;
    int j = (i + nt) % ns;
    int res = f(f, j);
    hops[i] = res == BIG ? BIG : res + 1;
    searching[i] = false;
    return *hops[i];
  };
  int ans = 0;
  REP(i, ns) { chmax(ans, f(f, i)); }
  return ans == BIG ? -1 : ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
