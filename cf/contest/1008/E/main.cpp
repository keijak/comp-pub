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

int query(i64 x, i64 y) {
  cout << x << " " << y << endl;
  int res;
  cin >> res;
  if (res == 0) exit(0);
  return res;
}

int main() {
  i64 n;
  cin >> n;
  i64 a_lb = 1, b_lb = 1;
  i64 ma_ub = n + 1, mi_ub = n + 1;
  //   {
  //     int res = query(1, 1);
  //     if (res == 1) {
  //       a_lb = 2;
  //     } else if (res == 2) {
  //       b_lb = 2;
  //     } else {
  //       assert(false);
  //     }
  //   }
  //   {
  //     int res = query(n, n);
  //     if (res == 3) {
  //       ma_ub = n;
  //     } else {
  //       assert(false);
  //     }
  //   }

  REP(iter, 600) {
    chmin(mi_ub, ma_ub);
    DEBUG(a_lb, b_lb, ma_ub, mi_ub);

    i64 ma_lb = max(a_lb, b_lb);
    if (ma_ub - ma_lb > 1) {
      i64 mid = (ma_ub + ma_lb) / 2;
      int res = query(mid, mid);
      if (res == 1) {
        a_lb = mid + 1;
      } else if (res == 2) {
        b_lb = mid + 1;
      } else {
        ma_ub = mid;
      }
      continue;
    }
    assert(ma_ub - ma_lb == 1);

    if (a_lb == b_lb) {
      query(a_lb, b_lb);
      break;
      //   assert(false);
    } else if (a_lb < b_lb) {
      if (mi_ub - a_lb <= 1) {
        query(a_lb, b_lb);
        break;
        // assert(false);
      }
      i64 mid = (mi_ub + a_lb) / 2;
      int res = query(mid, b_lb);
      if (res == 1) {
        a_lb = mid + 1;
      } else if (res == 3) {
        mi_ub = mid;
      } else {
        break;
        // assert(false);
      }
    } else {
      assert(a_lb > b_lb);
      if (mi_ub - b_lb <= 1) {
        query(a_lb, b_lb);
        break;
        // assert(false);
      }
      i64 mid = (mi_ub + b_lb) / 2;
      int res = query(a_lb, mid);
      if (res == 2) {
        b_lb = mid + 1;
      } else if (res == 3) {
        mi_ub = mid;
      } else {
        break;
        // assert(false);
      }
    }
  }

  // assert(false);
}
