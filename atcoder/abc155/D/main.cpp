#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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

i64 floordiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return abs(x) / abs(y);
  } else {
    i64 d = abs(y);
    i64 ceildiv = (abs(x) + d - 1) / d;
    return -ceildiv;
  }
}

i64 ceildiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    i64 d = abs(y);
    return (abs(x) + d - 1) / d;
  } else {
    i64 floordiv = abs(x) / abs(y);
    return -floordiv;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 n, k;
  cin >> n >> k;
  V<i64> A(n);
  cin >> A;
  sort(ALL(A));

  auto check = [&](i64 x) -> bool {
    i64 num_smaller = 0;
    for (int i = n - 1; i >= 0; --i) {
      if (A[i] == 0) {
        if (x > 0) num_smaller += i;
      } else if (A[i] > 0) {
        i64 y1 = ceildiv(x, A[i]);
        auto it1 = lower_bound(A.begin(), A.begin() + i, y1);
        int k1 = it1 - A.begin();
        assert(k1 <= i);
        num_smaller += k1;
      } else {
        i64 y2 = floordiv(x, A[i]);
        auto it2 = upper_bound(A.begin(), A.begin() + i, y2);
        int k2 = (A.begin() + i) - it2;
        num_smaller += k2;
      }
    }
    return num_smaller < k;
  };

  i64 tv = -1e18 - 5, fv = 1e18 + 5;
  while (fv - tv > 1) {
    i64 mid = floordiv(tv + fv, 2);
    (check(mid) ? tv : fv) = mid;
  }
  cout << tv << endl;
}
