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

const i64 INF = 1e18;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, W;
  cin >> N >> W;
  V<tuple<i64, i64>> vw(N);
  i64 max_value = 0, max_weight = 0;
  REP(i, N) {
    i64 v, w;
    cin >> v >> w;
    vw[i] = {v, w};
    chmax(max_value, v);
    chmax(max_weight, w);
  }

  auto solve_small_value = [&]() -> i64 {
    int mv = max_value * N;
    auto dp = vector(N + 1, vector(mv + 1, INF));
    dp[0][0] = 0;
    for (int i = 1; i <= N; ++i) {
      auto [vi, wi] = vw[i - 1];
      dp[i] = dp[i - 1];
      for (int v = 0; v <= mv - vi; ++v) {
        chmin(dp[i][v + vi], dp[i - 1][v] + wi);
      }
    }
    i64 ans = 0;
    for (int v = 0; v <= mv; ++v) {
      if (dp[N][v] <= W) ans = v;
    }
    return ans;
  };

  auto solve_small_weight = [&]() -> i64 {
    int mw = max_weight * N;
    auto dp = vector(N + 1, vector(mw + 1, -INF));
    dp[0][0] = 0LL;
    for (int i = 1; i <= N; ++i) {
      auto [vi, wi] = vw[i - 1];
      dp[i] = dp[i - 1];
      for (int w = mw - wi; w >= 0; w--) {
        if (dp[i - 1][w] < INF) {
          chmax(dp[i][w + wi], dp[i - 1][w] + vi);
        }
      }
    }
    i64 ans = 0;
    for (int w = 0; w <= min<i64>(mw, W); ++w) {
      chmax(ans, dp[N][w]);
    }
    return ans;
  };

  auto solve_small_n = [&]() -> i64 {
    const int n1 = N / 2;
    const int n2 = N - n1;
    map<i64, i64> mp;
    mp[0] = 0;
    REP(bits, 1 << n1) {
      i64 vsum = 0, wsum = 0;
      REP(i, n1) {
        if (bits >> i & 1) {
          auto [v, w] = vw[i];
          vsum += v;
          wsum += w;
        }
      }
      chmax(mp[wsum], vsum);
    }
    {
      auto it = mp.begin();
      while (it != mp.end()) {
        i64 pval = it->second;
        ++it;
        while (it != mp.end() and pval >= it->second) {
          it = mp.erase(it);
        }
      }
    }

    i64 ans = 0;
    REP(bits, 1 << n2) {
      i64 vsum = 0, wsum = 0;
      REP(i, n2) {
        if (bits >> i & 1) {
          auto [v, w] = vw[N - 1 - i];
          vsum += v;
          wsum += w;
        }
      }
      if (wsum > W) continue;
      auto it = mp.upper_bound(W - wsum);
      if (it != mp.begin()) {
        --it;
        i64 wc = wsum + it->first;
        i64 vc = vsum + it->second;
        if (wc <= W) {
          chmax(ans, vc);
        }
      }
    }
    return ans;
  };

  if (max_value <= 1000) {
    cout << solve_small_value() << '\n';
  } else if (max_weight <= 1000) {
    cout << solve_small_weight() << '\n';
  } else if (N <= 30) {
    cout << solve_small_n() << '\n';
  }
}
