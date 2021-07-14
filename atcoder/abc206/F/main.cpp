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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const int WIDTH = 105;
const int GMAX = 500;

auto solve() {
  int n;
  cin >> n;
  vector<pair<int, int>> lr(n);

  auto overlaps = [&](int i, int j) -> bool {
    auto [li, ri] = lr[i];
    auto [lj, rj] = lr[j];
    if (ri <= lj) return false;
    if (rj <= li) return false;
    return true;
  };
  vector<vector<int>> overlap(n);
  vector<int> lm(n), rm(n);
  REP(i, n) {
    int l, r;
    cin >> l >> r;
    lr[i] = {l, r};
  }
  REP(i, n) {
    auto [l, r] = lr[i];
    REP(j, n) {
      auto [lj, rj] = lr[j];
      if (overlaps(i, j)) {
        // overlap[i].push_back(j);
        chmin(l, lj);
        chmax(r, rj);
      }
    }
    lm[i] = l;
    rm[i] = r;
  }

  // REP(i, n) {
  //   auto [l, r] = lr[i];
  //   DUMP(i, l, r, overlap[i], lm[i], rm[i]);
  // }

  // dp[l][r] := nimber of [l,r)
  auto dp = vector(WIDTH, vector(WIDTH + 1, 0ULL));
  for (int width = 1; width <= WIDTH; ++width) {
    for (int l = 1; l + width <= WIDTH; ++l) {
      const int r = l + width;

      unordered_set<u64> nexts;
      for (int k = 0; k < n; ++k) {
        auto [lk, rk] = lr[k];
        if (l <= lk and rk <= r) {
          u64 lg = dp[l][lk];
          u64 rg = dp[rk][r];
          u64 g = lg ^ rg;
          nexts.insert(g);
        }
      }
      u64 mex;
      for (u64 x = 0;; ++x) {
        if (not nexts.count(x)) {
          mex = x;
          break;
        }
      }
      dp[l][r] = mex;
    }
  }
  return dp[1][101];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  cin >> t;
  REP(test_case, t) {
    DUMP(test_case);
    auto ans = solve();
    DUMP(ans);
    cout << (ans != 0 ? "Alice" : "Bob") << "\n";
  }
}
