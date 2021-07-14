#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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

#ifdef MY_DEBUG
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

V<V<pair<int, int>>> solve() {
  int n, m;
  cin >> n >> m;
  V<string> s(n);
  REP(i, n) {
    cin >> s[i];
    assert(ssize(s[i]) == m);
  }

  V<V<pair<int, int>>> ops;
  auto flip = [&](int i, int j, V<pair<int, int>> &v) -> void {
    int b = s[i][j] - '0';
    s[i][j] = (1 - b) + '0';
    v.emplace_back(i, j);
  };
  auto modify = [&](int i, int j) -> void {
    array<int, 4> dx = {0, 0, 1, 1}, dy = {0, 1, 0, 1};
    int num = 0;
    REP(k, 4) { num += s[i + dx[k]][j + dy[k]] - '0'; }
    if (num == 0) return;
    if (num == 4) {
      V<pair<int, int>> v;
      REP(k, 3) { flip(i + dx[k], j + dy[k], v); }
      ops.push_back(move(v));
      num = 1;
    }
    if (num == 1) {
      V<pair<int, int>> v;
      REP(k, 4) {
        if (s[i + dx[k]][j + dy[k]] == '1') {
          flip(i + dx[(k + 3) % 4], j + dy[(k + 3) % 4], v);
          flip(i + dx[k], j + dy[k], v);
          flip(i + dx[(k + 1) % 4], j + dy[(k + 1) % 4], v);
          break;
        }
      }
      ops.push_back(move(v));
      num = 2;
    }
    if (num == 2) {
      V<pair<int, int>> v;
      REP(k, 4) {
        int cnt = 0;
        REP(q, 4) {
          if (q == k) continue;
          cnt += s[i + dx[q]][j + dy[q]] == '1';
        }
        if (cnt == 1) {
          REP(q, 4) {
            if (q == k) continue;
            flip(i + dx[q], j + dy[q], v);
          }
          break;
        }
      }
      assert(ssize(v) == 3);
      ops.push_back(move(v));
      num = 3;
    }
    if (num == 3) {
      V<pair<int, int>> v;
      REP(k, 4) {
        if (s[i + dx[k]][j + dy[k]] == '0') continue;
        flip(i + dx[k], j + dy[k], v);
      }
      ops.push_back(move(v));
      // DEBUG(ops);
    }
  };

  for (int i = 0; i < n - 1; i += 2) {
    for (int j = 0; j < m - 1; j++) {
      if (s[i][j] == '0' and s[i + 1][j] == '0') continue;
      modify(i, j);
    }
    if (s[i][m - 1] == '1' or s[i + 1][m - 1] == '1') {
      modify(i, m - 2);
    }
  }
  for (int j = 0; j < m - 1; j++) {
    if (s[n - 2][j] == '0' and s[n - 1][j] == '0') continue;
    modify(n - 2, j);
  }
  if (s[n - 2][m - 1] == '1' or s[n - 1][m - 1] == '1') {
    modify(n - 2, m - 2);
  }
  return ops;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  REP(i, t) {
    auto res = solve();
    cout << ssize(res) << '\n';
    for (auto &pairs : res) {
      assert(ssize(pairs) == 3);
      for (auto [x, y] : pairs) {
        cout << x + 1 << ' ' << y + 1 << ' ';
      }
    }
    cout << '\n';
  }
}
