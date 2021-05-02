#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

struct Cluster {
  int x;
  int y;
  i64 pop;
};

const i64 BIG = 1e17;

void solve() {
  int n;
  cin >> n;

  vector<Cluster> clusters(n);
  REP(i, n) {
    Cluster c;
    cin >> c.x >> c.y >> c.pop;
    clusters[i] = move(c);
  }

  auto f = [&](const vector<pair<int, i64>> &cs) -> vector<i64> {
    const int m = ssize(cs);
    vector<i64> res(n + 1, BIG);
    for (int i = m; i <= n; ++i) {
      res[i] = 0;
    }
    if (m == 0) return res;

    vector dp(m + 1, vector(m + 1, BIG));
    dp[0][0] = 0;
    for (int r = 1; r <= m; ++r) {
      for (int l = 0; l < r; ++l) {
        {
          i64 c = 0;
          for (int i = l; i < r; ++i) {
            c += abs(cs[i].first) * cs[i].second;
          }
          REP(j, m + 1) { chmin(dp[r][j], dp[l][j] + c); }
        }
        i64 cost = BIG;
        for (int p = l; p < r; ++p) {
          i64 c = 0;
          for (int i = l; i < r; ++i) {
            c += abs(cs[i].first - cs[p].first) * cs[i].second;
          }
          chmin(cost, c);
        }
        REP(j, m) { chmin(dp[r][j + 1], dp[l][j] + cost); }
      }
    }
    REP(j, m) chmin(res[j], dp[m][j]);
    return res;
  };

  vector<i64> costs(n + 1, BIG);
  REP(bits_h, 1 << n) {
    vector<pair<int, i64>> hc, vc;
    REP(i, n) {
      if (bits_h & (1 << i)) {
        hc.push_back({clusters[i].x, clusters[i].pop});
      } else {
        vc.push_back({clusters[i].y, clusters[i].pop});
      }
    }
    sort(ALL(hc));
    sort(ALL(vc));
    auto hcosts = f(hc);
    auto vcosts = f(vc);
    for (int k = 0; k <= n; ++k) {
      for (int i = 0; i <= k; ++i) {
        chmin(costs[k], hcosts[i] + vcosts[k - i]);
      }
    }
  }

  REP(i, n + 1) { cout << costs[i] << "\n"; }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
