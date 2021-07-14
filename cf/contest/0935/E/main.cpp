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

struct Node {
  vector<i64> maxval;
  vector<i64> minval;
};

const i64 INF = 1e15;

i64 solve() {
  string E;
  cin >> E;
  int n = ssize(E);
  int P, M;
  cin >> P >> M;
  bool plus_state = true;
  int limit = P;
  if (M < P) {
    plus_state = false;
    limit = M;
  }

  auto parse = [&](auto rec, int i) -> pair<Node, int> {
    char ch = E[i];
    if (ch == '(') {
      auto [a1, j] = rec(rec, i + 1);
      assert(E[j] == '?');
      auto [a2, k] = rec(rec, j + 1);
      assert(E[k] == ')');
      Node res;
      res.maxval.assign(limit + 1, -INF);
      res.minval.assign(limit + 1, INF);
      if (plus_state) {
        for (int p = 0; p <= limit; ++p) {
          for (int q = 0; p + q <= limit; ++q) {
            chmax(res.maxval[p + q], a1.maxval[p] - a2.minval[q]);
            chmin(res.minval[p + q], a1.minval[p] - a2.maxval[q]);
          }
          for (int q = 0; p + q + 1 <= limit; ++q) {
            chmax(res.maxval[p + q + 1], a1.maxval[p] + a2.maxval[q]);
            chmin(res.minval[p + q + 1], a1.minval[p] + a2.minval[q]);
          }
        }
      } else {
        for (int p = 0; p <= limit; ++p) {
          for (int q = 0; p + q <= limit; ++q) {
            chmax(res.maxval[p + q], a1.maxval[p] + a2.maxval[q]);
            chmin(res.minval[p + q], a1.minval[p] + a2.minval[q]);
          }
          for (int q = 0; p + q + 1 <= limit; ++q) {
            chmax(res.maxval[p + q + 1], a1.maxval[p] - a2.minval[q]);
            chmin(res.minval[p + q + 1], a1.minval[p] - a2.maxval[q]);
          }
        }
      }
      return {move(res), k + 1};
    } else {
      int d = ch - '0';
      DEBUG(i, i + 1, d);
      Node res;
      res.maxval.assign(limit + 1, -INF);
      res.minval.assign(limit + 1, INF);
      res.maxval[0] = res.minval[0] = d;
      return {move(res), i + 1};
    }
  };
  auto [t, i] = parse(parse, 0);
  assert(i == n);
  return t.maxval[limit];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
