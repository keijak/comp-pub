#include <bits/stdc++.h>
using i128 = __int128_t;
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

void solve() {
  int n;
  cin >> n;
  V<i64> arr(n);
  cin >> arr;

  i64 mx = 0;
  for (int i = 0; i < n; i++) {
    chmax(mx, arr[i]);
  }

  vector<vector<i64>> mul(mx + 1);
  for (int i = 0; i < n; i++) {
    if (mul[arr[i]].size() > 1) {
      // we already found two
      // smallest multiple
      continue;
    }
    mul[arr[i]].push_back(arr[i]);
  }

  // iterating over all gcd
  for (i64 i = 1; i <= mx; i++) {
    // iterating over its multiple
    for (i64 j = i + i; j <= mx; j += i) {
      if (mul[i].size() > 1) {
        // if we already found the
        // two smallest multiple of i
        break;
      }
      for (i64 k : mul[j]) {
        if (mul[i].size() > 1) break;
        mul[i].push_back(k);
      }
    }
  }

  i128 mlcm = 1e18;
  i64 ma = -1, mb = -1;
  for (i64 i = 1; i <= mx; i++) {
    if (mul[i].size() <= 1) continue;

    // choosing smallest two multiple
    i64 a = mul[i][0], b = mul[i][1];

    // calculating lcm
    i128 lcm = ((i128)(a)*b) / i;

    if (chmin(mlcm, lcm)) {
      ma = a;
      mb = b;
    }
  }

  DEBUG(ma, mb);
  int ia = -1, ib = -1;
  REP(i, n) {
    if (arr[i] == ma and ia < 0) {
      ia = i + 1;
      continue;
    }
    if (arr[i] == mb and ib < 0) {
      ib = i + 1;
    }
  }
  cout << min(ia, ib) << ' ' << max(ia, ib) << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
}
