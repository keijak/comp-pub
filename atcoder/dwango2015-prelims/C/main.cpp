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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
using Float = long double;

Float factorial(int x) {
  assert(x >= 0);
  static std::vector<Float> facts = {1, 1, 2};
  while ((int)facts.size() <= x) {
    auto v = facts.back() * facts.size();
    facts.push_back(v);
  }
  return facts[x];
}

Float solve() {
  int n;
  cin >> n;
  vector<Float> dp(n + 1);
  dp[1] = 0;
  for (int k = 2; k <= n; ++k) {
    vector<int> balls(k + 2, 0);
    balls[k + 1] = balls[k] = 1;
    vector<Float> nex(k + 1, 0);
    do {
      vector<pair<int, int>> a = {pair{0, 0}, pair{0, 1},
                                  pair{k + 2, 2}};  // G,C,P
      while (balls[a[0].first] != 1) ++a[0].first;
      a[1].first = a[0].first + 1;
      while (balls[a[1].first] != 1) ++a[1].first;
      a[2].first -= a[1].first + 1;
      a[1].first -= a[0].first + 1;
      assert(a[0].first + a[1].first + a[2].first == k);
      Float p = factorial(k) / pow(Float(3), k) / factorial(a[0].first) /
                factorial(a[1].first) / factorial(a[2].first);
      vector<pair<int, int>> b(ALL(a));
      sort(ALL(b));
      if (b[0].first == b[1].first and b[1].first == b[2].first) {
        nex[k] += p;
      } else if (b[0].first == 0 and b[1].first == 0) {
        nex[k] += p;
      } else {
        auto it = b.begin();
        while (it->first == 0) ++it;
        assert(it != b.end());
        auto nit = next(it);
        assert(nit != b.end());
        if (it->first < nit->first) {
          nex[it->first] += p;
        } else if ((it->second + 1) % 3 == nit->second) {
          nex[it->first] += p;
        } else {
          assert((nit->second + 1) % 3 == it->second);
          nex[nit->first] += p;
        }
      }
    } while (next_permutation(ALL(balls)));

    Float val = 0;
    for (int i = 1; i < k; ++i) {
      val += nex[i] * (dp[i] + 1);
    }
    val += nex[k];
    val /= 1 - nex[k];
    dp[k] = val;
  }
  return dp[n];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}
