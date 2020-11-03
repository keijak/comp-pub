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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 n, k;
  cin >> n >> k;
  V<i64> a(n);
  cin >> a;
  sort(ALL(a));
  i64 amin = a[0];
  i64 amax = a[n - 1];
  DEBUG(amax, amin);

  V<int> lt(n), gt(n);
  lt[0] = -1;
  gt[n - 1] = n;
  for (int i = 1; i < n; ++i) {
    if (a[i - 1] < a[i]) {
      lt[i] = i - 1;
    } else {
      lt[i] = lt[i - 1];
    }
    int ri = n - 1 - ri;
    if (a[ri] < a[ri + 1]) {
      gt[ri] = ri + 1;
    } else {
      gt[ri] = gt[ri + 1];
    }
  }

  auto check = [&](i64 x) -> bool {
    int i = 0, j = n - 1;
    int rem = k;
    for (; i <= j;) {
      if (a[j] - a[i] <= x) return true;
      int i2 = gt[i];
      int j2 = lt[j];
      int i2 = i, j2 = j;
      while (i2 < j and a[i2] == a[i]) ++i2;
      while (j2 > i and a[j2] == a[j]) --j2;
    }
  };

  i64 fv = -1, tv = amax - amin;
  while (tv - fv > 1) {
    i64 mid = (tv + fv) / 2;
    (check(mid) ? tv : fv) = mid;
  }
  cout << tv << endl;
}
