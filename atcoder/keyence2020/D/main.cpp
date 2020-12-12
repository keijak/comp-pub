#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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

const int INF = 1e9;

i64 solve() {
  int n;
  cin >> n;
  vector<int> a(n), b(n);
  cin >> a >> b;

  int total_even_pos = (n + 1) / 2;
  vector<int> val(n);
  vector<bool> even(n, false);
  int ans = INF;
  REP(bits, 1 << n) {
    int even_count = 0;
    REP(i, n) {
      bool red;
      if (bits >> i & 1) {
        red = true;
        val[i] = a[i];
      } else {
        red = false;
        val[i] = b[i];
      }
      even[i] = red ^ (i % 2 == 1);
      if (even[i]) ++even_count;
    }
    if (even_count != total_even_pos) continue;

    vector<int> sorted = val;
    sort(ALL(sorted));
    int swap_count = 0;
    REP(i, n) {
      int x = -1;
      for (int j = i; j < n; ++j) {
        if (val[j] != sorted[i]) continue;
        if (even[j] != (i % 2 == 0)) continue;
        x = j;
        break;
      }
      if (x == -1) goto cont;
      while (x > i) {
        swap(val[x], val[x - 1]);
        swap(even[x], even[x - 1]);
        --x;
        ++swap_count;
      }
    }
    assert(val == sorted);
    chmin(ans, swap_count);

  cont:
    continue;
  }

  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  cout << (ans == INF ? -1 : ans) << "\n";
}
