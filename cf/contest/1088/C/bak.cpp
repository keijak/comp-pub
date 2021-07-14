#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(nextval) std::begin(nextval), std::end(nextval)
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
  for (auto &nextval : a) is >> nextval;
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
const int DMAX = 1000000;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;

  int h = DMAX;
  vector<array<int, 3>> op;
  REP(i, 2) { op.push_back({1, n, h}); }
  REP(i, n) { a[i] += h * 2; }

  i64 nextval = a[n - 1];
  for (int i = n - 2; i >= 0; nextval = a[i--]) {
    if (a[i] == nextval - 1) {
      continue;
    }
    if (a[i] < nextval - 1) {
      i64 delta = (nextval - 1) - a[i];
      if (delta > DMAX) {
        delta = DMAX;
      }
      op.push_back({1, i + 1, int(delta)});
      for (int j = 0; j <= i; ++j) {
        a[j] += delta;
      }
      continue;
    }
    assert(a[i] >= 0);
    for (i64 d = 2;; ++d) {
      i64 q = (a[i] + d) / d;
      if (q > DMAX) continue;
      i64 y = a[i] % q;
      if (y < nextval) {
        op.push_back({2, i + 1, int(q)});
        for (int j = 0; j <= i; ++j) {
          a[j] %= q;
        }
        break;
      }
      int delta = y - (nextval - 1);
      if (q + delta <= DMAX) {
        op.push_back({2, i + 1, int(q + delta)});
        y = a[i] % (q + delta);
        for (int j = 0; j <= i; ++j) {
          a[j] %= (q + delta);
        }
        break;
      }
    }
  }

  DEBUG(a);
  cout << ssize(op) << "\n";
  for (auto [b, i, v] : op) {
    cout << b << " " << i << " " << v << "\n";
  }
}
