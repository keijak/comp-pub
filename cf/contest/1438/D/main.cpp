#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
using V = std::vector<T>;
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

optional<V<array<int, 3>>> solve() {
  int n;
  cin >> n;
  V<uint32_t> a(n);
  cin >> a;
  uint32_t target = 0;
  REP(i, n) { target ^= a[i]; }
  int m = n;
  if (not(n & 1)) {
    if (target != 0) return nullopt;
    target = a[n - 1];
    m = n - 1;
  }
  V<array<int, 3>> res;
  int j = 0;
  array<int, 3> t;
  for (; j + 2 < m;) {
    DEBUG(a);
    for (int k = 0; k < 3 and j < m;) {
      t[k++] = j++;
    }
    res.push_back(t);
    uint32_t x = a[t[0]] ^ a[t[1]] ^ a[t[2]];
    a[t[0]] = a[t[1]] = a[t[2]] = x;
    --j;
  }
  assert(a[n - 1] == target);
  j = 0;
  while (j + 1 < n - 1) {
    DEBUG(a);
    t[2] = n - 1;
    for (int k = 0; k < 2 and j < n - 1;) {
      t[k++] = j++;
    }
    assert(a[t[0]] == a[t[1]]);
    res.push_back(t);
    uint32_t x = a[t[0]] ^ a[t[1]] ^ a[t[2]];
    a[t[0]] = a[t[1]] = a[t[2]] = x;
  }
  DEBUG(a);
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  auto res = solve();
  if (not res.has_value()) {
    cout << "NO\n";
  } else {
    cout << "YES\n";
    cout << res.value().size() << '\n';
    for (auto &v : res.value()) {
      cout << v[0] + 1 << ' ' << v[1] + 1 << ' ' << v[2] + 1 << '\n';
    }
  }
}
