#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

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

#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
using namespace std;

struct ProblemSets {
  i64 maxSets(i64 E, i64 EM, i64 M, i64 MH, i64 H) {
    auto check = [&](i64 x) -> bool {
      array<i64, 3> a = {E, M, H};
      i64 em = EM, mh = MH;
      if (a[0] < x) {
        i64 d = x - a[0];
        if (em < d) return false;
        em -= d;
      }
      if (a[2] < x) {
        i64 d = x - a[2];
        if (mh < d) return false;
        mh -= d;
      }
      if (a[1] < x) {
        i64 d = x - a[1];
        if (em + mh < d) return false;
      }
      return true;
    };

    u64 tv = 0, fv = u64(2e18) + 100;
    while (fv - tv > 1) {
      u64 mid = (fv + tv) / 2;
      (check(mid) ? tv : fv) = mid;
    }
    return tv;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  ProblemSets solver;
  i64 a = solver.maxSets(2, 2, 1, 2, 2);
  DEBUG(a);
  assert(a == 3);

  a = solver.maxSets(100, 100, 100, 0, 0);
  DEBUG(a);
  assert(a == 0);

  a = solver.maxSets(657, 657, 657, 657, 657);
  DEBUG(a);
  assert(a == 1095);

  a = solver.maxSets(1000000000000000000LL, 1000000000000000000LL,
                     1000000000000000000LL, 1000000000000000000LL,
                     1000000000000000000LL);
  DEBUG(a);
  assert(a == 1666666666666666666);
}
