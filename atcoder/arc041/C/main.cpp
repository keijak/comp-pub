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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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
  V<int> X(N);
  V<char> D(N);
  REP(i, N) { cin >> X[i] >> D[i]; }

  V<V<int>> ls, rs;
  for (int i = 0; i < N;) {
    rs.emplace_back(0);
    auto &rv = rs.back();
    while (i < N and D[i] == 'R') {
      rv.push_back(i++);
    }
    ls.emplace_back(0);
    auto &lv = ls.back();
    while (i < N and D[i] == 'L') {
      lv.push_back(i++);
    }
  }
  int M = ls.size();
  assert((int)rs.size() == M);
  DEBUG(rs);
  DEBUG(ls);

  i64 ans = 0;
  int lmin, rmax;
  REP(i, M) {
    if (rs[i].size() <= ls[i].size()) {
      if (rs[i].empty()) {
        lmin = 1;
      } else {
        lmin = X[rs[i].back()] + 1;
        rmax = X[rs[i].back()] - 1;
        for (int j = (int)rs[i].size() - 2; j >= 0; --j) {
          ans += rmax - X[rs[i][j]];
          rmax--;
        }
      }
      for (int j : ls[i]) {
        assert(lmin <= X[j]);
        ans += X[j] - lmin;
        lmin++;
      }
    } else {
      if (ls[i].empty()) {
        rmax = L;
      } else {
        rmax = X[ls[i].front()] - 1;
        lmin = X[ls[i].front()] + 1;
        for (int j = 1; j < (int)ls[i].size(); ++j) {
          ans += X[ls[i][j]] - lmin;
          lmin++;
        }
      }
      for (int j = (int)rs[i].size() - 1; j >= 0; --j) {
        ans += rmax - X[rs[i][j]];
        rmax--;
      }
    }
  }
  cout << ans << endl;
}
