#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename V>
void transpose(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  std::vector<V> tmp(w, V(h, grid[0][0]));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}
template<typename V>
void transpose(std::vector<V> &grid, int &H, int &W) {
  transpose(grid);
  std::swap(H, W);
}

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

template<typename T>
struct Compressed {
  const std::vector<T> &values;

  explicit Compressed(const std::vector<T> &v) : values(v) {}

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

Mint solve() {
  int H = in, W = in;
  vector<string> g(H);
  REP(i, H) cin >> g[i];
  if (H < W) {
    transpose(g, H, W);
  }
  if (W == 1) {
    auto dp = vector(H, vector(2, Mint(0)));
    dp[0][0] = 1;
    dp[0][1] = (g[0][0] == '.') ? 1 : 0;
    REP(i, 1, H) {
      dp[i][0] = dp[i - 1][0] + dp[i - 1][1];
      if (g[i][0] == '.') {
        dp[i][1] = dp[i - 1][0];
      }
    }
    return dp[H - 1][0] + dp[H - 1][1];
  }

  vector<uint32_t> bitmap;
  REP(bits, 1 << (W + 1)) {
    bool good = [&]() {
      if (has_bit(bits, W) and has_bit(bits, 0)) return false;
      int repeated_cnt = 0;
      REP(i, 1, W) {
        if (has_bit(bits, i) and has_bit(bits, i - 1)) ++repeated_cnt;
      }
      return repeated_cnt <= 1;
    }();
    if (good) bitmap.push_back(bits);
  }
  Compressed<uint32_t> cbitmap(bitmap);
  const int B = cbitmap.size();
  vector<array<optional<pair<uint32_t, int>>, 2>> nexts(B);
  REP(b, B) {
    uint32_t mask = bitmap[b] >> 1;
    int mi = cbitmap.index(mask);
    if (cbitmap.value(mi) == mask) {
      nexts[b][0] = {mask, mi};

    }
    uint32_t mask2 = mask | (1 << W);
    int mi2 = cbitmap.index(mask2);
    if (cbitmap.value(mi2) == mask2) {
      nexts[b][1] = {mask2, mi2};
    }
  }

  auto dp = vector(3, vector(W, vector(B, Mint(0))));

  REP(i, B) {
    const uint32_t bits = bitmap[i];
    bool good_init = [&]() {
      if (has_bit(bits, W) and g[1][0] == '#') return false;
      REP(i, W) {
        if (has_bit(bits, i) and g[0][i] == '#') return false;
      }
      REP(i, 1, W) {
        if (has_bit(bits, i) and has_bit(bits, i - 1)) return false;
      }
      if (has_bit(bits, W) and has_bit(bits, 0)) return false;
      if (has_bit(bits, W) and has_bit(bits, 1)) return false;
      return true;
    }();
    if (not good_init) continue;
    dp[1][0][i] = 1;
  }
  REP(x, 1, H) {
    dp[(x + 1) % 3].assign(W, vector<Mint>(B, Mint(0)));
    REP(y, W) {
      if (x == H - 1 and y == W - 1) break;
      REP(b, B) {
        const uint32_t bits = bitmap[b];
        Mint now = dp[x % 3][y][b];
        if (now.val() == 0) continue;
        const auto &ma0 = nexts[b][0];
        const auto &ma1 = nexts[b][1];
        if (y == W - 1) {
          if (ma0.has_value()) {
            dp[(x + 1) % 3][0][ma0->second] += now;
          }
          if (g[x + 1][0] == '.' and not has_bit(bits, 1) and not has_bit(bits, 2)) {
            if (ma1.has_value()) {
              dp[(x + 1) % 3][0][ma1->second] += now;
            }
          }
        } else {
          if (ma0.has_value()) {
            dp[x % 3][y + 1][ma0->second] += now;
          }
          if (g[x][y + 1] == '.' and not has_bit(bits, 0) and not has_bit(bits, 1)
              and (y + 1 == W - 1 or not has_bit(bits, 2)) and not has_bit(bits, W)) {
            if (ma1.has_value()) {
              dp[x % 3][y + 1][ma1->second] += now;
            }
          }
        }
      }
    }
  }

  Mint ans = 0;
  REP(b, B) {
    ans += dp[(H - 1) % 3][W - 1][b];
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { print(solve()); }
  exit_();
}
