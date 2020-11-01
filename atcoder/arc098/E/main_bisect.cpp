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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K, Q;
  cin >> N >> K >> Q;
  V<i64> A(N);
  cin >> A;

  V<i64> sa = A;
  sort(ALL(sa));

  auto check = [&](i64 x) -> bool {
    REP(i, N) {
      i64 minval = sa[i];
      if (i > 0 and minval == sa[i - 1]) continue;
      int ub = upper_bound(ALL(sa), minval + x) - sa.begin();
      if (ub - i < Q) continue;
      i64 ub_val = 1e18;
      if (ub < N) ub_val = sa[ub];

      i64 rem = Q;
      int over = 0, legit = 0;
      for (int j = 0; j <= N; ++j) {
        i64 aval = (j < N) ? A[j] : -1;
        if (aval < minval) {
          const int gap = legit + over;
          if (gap >= K) {
            i64 consumed = min(gap - K + 1, legit);
            rem -= consumed;
            if (rem <= 0) return true;
          }
          over = legit = 0;
        } else if (aval < ub_val) {
          legit++;
        } else {
          over++;
        }
      }
    }
    return false;
  };

  i64 fv = -1, tv = 1e10;
  while (tv - fv > 1) {
    i64 mid = (fv + tv) / 2;
    (check(mid) ? tv : fv) = mid;
  }
  cout << tv << endl;
}
