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

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> par(n, -1);
  vector<vector<int>> child(n);
  REP(i, n - 1) {
    int p;
    cin >> p;
    --p;
    par[i + 1] = p;
    child[p].push_back(i + 1);
  }

  auto dfs = [&](auto rec, int v) -> tuple<i64, bool> {
    if (child[v].empty()) {
      return {-1, true};
    }
    i64 rs = -1;
    i64 neg = 0;
    vector<i64> s;
    for (auto u : child[v]) {
      auto [ps, flip] = rec(rec, u);
      if (not flip) {
        if (ps >= 0) {
          rs += ps;
        } else {
          neg += ps;
        }
      } else {
        s.push_back(ps);
      }
    }
    sort(ALL(s), greater<>());
    REP(i, ssize(s)) {
      if (i % 2 == 0) {
        rs += s[i];
      } else {
        rs -= s[i];
      }
    }
    if (ssize(s) % 2 == 0) {
      rs += neg;
    } else {
      rs -= neg;
    }
    bool flip = ssize(s) % 2 == 0;
    return {rs, flip};
  };
  auto [delta, flip] = dfs(dfs, 0);
  i64 ao = n + delta;
  i64 tak = n - delta;
  DEBUG(ao, tak);
  cout << (tak / 2) << endl;
}
