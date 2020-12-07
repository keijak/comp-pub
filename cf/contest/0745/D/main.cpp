#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
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

vector<i64> solve(int n) {
  const i64 INF = 1e10;
  vector<i64> ans(n, INF);

  REP(b, 10) {
    REP(on, 2) {
      int bit = 1 << b;
      vector<int> q;
      for (int j = 1; j <= n; ++j) {
        bool bit_on = j & bit;
        if (bit_on == bool(on)) {
          q.push_back(j);
        }
      }
      int k = ssize(q);
      if (k == n or k == 0) continue;
      cout << k << endl;
      REP(j, k) {
        if (j > 0) cout << ' ';
        cout << q[j];
      }
      cout << endl;

      for (int j = 1; j <= n; ++j) {
        i64 resp;
        cin >> resp;
        bool bit_on = j & bit;
        if (bit_on == bool(on)) {
          assert(resp == 0);
        } else {
          chmin(ans[j - 1], resp);
        }
      }
    }
  }
  return ans;
}

int main() {
  int n;  // <= 1000
  cin >> n;
  auto ans = solve(n);
  cout << "-1" << endl;
  REP(i, n) {
    if (i > 0) cout << ' ';
    cout << ans[i];
  }
  cout << endl;
  fflush(stdout);
}
