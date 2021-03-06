#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
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
  for (auto rit = std::begin(a); rit != e; ++rit) {
    if (rit != b) *os << sep;
    *os << *rit;
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

  int N, L;
  cin >> N >> L;

  V<i64> ax(N + 2), bx(N + 2);
  ax[0] = bx[0] = 0;
  ax[N + 1] = bx[N + 1] = L + 1;
  REP(i, N) { cin >> ax[i + 1]; }
  REP(i, N) { cin >> bx[i + 1]; }

  auto solve = [&]() -> i64 {
    set<int> resolved;
    REP(i, N + 2) {
      if (ax[i] == bx[i]) {
        resolved.insert(i);
      }
    }

    set<pair<int, int>> zmap;
    REP(i, N + 2) { zmap.emplace(ax[i] - i, i); }

    i64 count = 0;

    for (int i = 1; i <= N;) {
      if (ax[i] >= bx[i]) {
        ++i;
        continue;
      }
      auto rit = resolved.upper_bound(i);
      i64 zi = bx[i] - i;
      auto zit = zmap.upper_bound({zi, i});
      if (zit == zmap.end()) return -1;
      auto [zj, j] = *zit;
      if (zi != zj) return -1;
      if (j > *rit) return -1;
      count += j - i;
      while (i < j and bx[i] - i == zi) {
        resolved.insert(i);
        ++i;
      }
    }
    for (int i = N; i >= 1;) {
      if (resolved.count(i)) {
        --i;
        continue;
      }
      assert(ax[i] > bx[i]);
      auto rit = resolved.lower_bound(i);
      --rit;
      i64 zi = bx[i] - i;
      auto zit = zmap.lower_bound({zi, i});
      if (zit == zmap.begin()) return -1;
      --zit;
      auto [zj, j] = *zit;
      if (zi != zj) return -1;
      if (j < *rit) return -1;
      count += i - j;
      while (i > j and bx[i] - i == zi) {
        resolved.insert(i);
        --i;
      }
    }
    return count;
  };

  cout << solve() << endl;
}
