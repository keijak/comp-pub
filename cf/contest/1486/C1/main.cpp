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

int main() {
  ios_base::sync_with_stdio(false);

  int n;
  cin >> n;

  map<array<int, 2>, int> cache;
  int counter = 0;

  auto query = [&](int l, int r) -> int {
    array<int, 2> key{l, r};
    if (cache.count(key)) return cache[key];
    assert(counter < 20);
    cout << "? " << l << " " << r << endl;
    ++counter;
    int x;
    cin >> x;
    cache[key] = x;
    return x;
  };

  auto ask = [&](int l, int r, int sec) -> bool {
    int x = query(l, r);
    return x == sec;
  };

  int lo = 1, hi = n;

  while (hi - lo > 0) {
    int x = query(lo, hi);
    if (hi - lo == 1) {
      if (lo == x) {
        lo++;
      } else {
        hi--;
      }
      break;
    }

    if (hi - lo <= 9) {
      int m = (lo + hi) / 2;
      DEBUG(m, lo, hi);
      if (x <= m) {
        if (ask(lo, m, x)) {
          hi = m;
        } else {
          lo = m + 1;
        }
      } else {
        if (ask(m, hi, x)) {
          lo = m;
        } else {
          hi = m - 1;
        }
      }
      continue;
    }

    i64 ll = (lo + lo + hi) / 3;
    i64 rr = (lo + hi + hi) / 3;

    // int m = (lo + hi) / 2;
    // DEBUG(m, lo, hi);
    if (x <= rr) {
      if (ask(lo, rr, x)) {
        hi = rr;
      } else {
        lo = rr + 1;
      }
    } else {
      if (ask(ll, hi, x)) {
        lo = ll;
      } else {
        hi = ll - 1;
      }
    }
    // cerr << "## " << lo << " " << hi << endl;
  }
  cout << "! " << lo << endl;
}
