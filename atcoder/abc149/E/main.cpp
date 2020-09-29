#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
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
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
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

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int n;
  i64 m;
  cin >> n >> m;
  V<i64> A(n);
  cin >> A;
  sort(A.rbegin(), A.rend());

  auto check = [&](i64 x) -> bool {
    i64 r = m;
    REP(i, n) {
      if (2 * A[i] < x) break;
      r--;
      i64 y = x - A[i];
      int j = upper_bound(A.begin(), A.end(), y, greater<i64>()) - A.begin();
      i64 k = j - (i + 1);
      if (k > 0) {
        r -= 2 * k;
      }
      if (r <= 0) return true;
    }
    return false;
  };

  // There are M or more values greater than or equal to tv.
  i64 tv = 0, fv = 200001;
  while (fv - tv > 1) {
    i64 mid = (tv + fv) / 2;
    (check(mid) ? tv : fv) = mid;
  }
  DEBUG(tv);

  V<i64> cum(n + 1);
  REP(i, n) { cum[i + 1] = cum[i] + A[i]; }

  {
    i64 ans = 0;
    i64 least = tv;
    i64 r = m;
    REP(i, n) {
      i64 x = A[i];
      if (2 * x <= least) continue;
      ans += 2 * x;
      r--;
      i64 y = least - x;
      int j = lower_bound(A.begin(), A.end(), y, greater<i64>()) - A.begin();
      i64 k = j - (i + 1);
      if (k > 0) {
        ans += 2 * (cum[j] - cum[i + 1] + k * x);
        r -= 2 * k;
      }
    }
    if (r >= 0) {
      ans += r * least;
    }
    cout << ans << endl;
  }
}
