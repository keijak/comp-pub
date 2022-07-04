// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<unsigned M>
struct ModInt {
  constexpr ModInt() : _v(0) {}
  constexpr ModInt(long long val) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }

  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long) (_v) * a._v) % M;
    return *this;
  }
  constexpr ModInt pow(long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
    }
    if (t < 0) {
      return this->inv().pow(-t);
    }
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  ModInt inv() const {
    static constexpr unsigned kPrecalcSize = 10000;
    static const std::array<ModInt, kPrecalcSize> *kPrecalc = []() {
      auto *a = new std::array<ModInt, kPrecalcSize>;
      for (ModInt i = 1; i.val() < kPrecalcSize; i += 1) {
        (*a)[i.val()] = i.inv_internal();
      }
      return a;
    }();
    if (_v < kPrecalcSize) return (*kPrecalc)[_v];
    return inv_internal();
  }
  ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r += b;
    return r;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r -= b;
    return r;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r *= b;
    return r;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    ModInt r = a;
    r /= b;
    return r;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  ModInt inv_internal() const {
    long long b = 1, a = _v;
    while (a > 1) {
      long long q = M / a;
      a = M - a * q;
      b = -b * q % M;
    }
    assert(a == 1);  // if a = 0, _v and M are not coprime.
    if (b < 0) b += M;
    ModInt ret;
    ret._v = (unsigned) b;
    return ret;
  }

  unsigned _v;  // raw value
};
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

int countPaths(vector<vector<int>> &grid) {
  const int m = (int) grid.size();
  const int n = (int) grid[0].size();
  auto inside = [&](int x, int y) {
    return 0 <= x and x < m and 0 <= y and y < n;
  };
  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[] = {1, 0, -1, 0};
    static const int dy[] = {0, 1, 0, -1};
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  auto dp = vector(m, vector(n, Mint(0)));
  map<int, vector<pair<int, int>>> table;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      table[grid[i][j]].push_back({i, j});
    }
  }
  for (auto it = table.rbegin(); it != table.rend(); ++it) {
    const vector<pair<int, int>> &idx = it->second;

    for (auto [x, y]: idx) {
      int me = grid[x][y];
      Mint path_count = 1;
      for (auto [nx, ny]: adjacent(x, y)) {
        int neighbor = grid[nx][ny];
        if (neighbor > me) {
          path_count += dp[nx][ny];
        }
      }
      dp[x][y] = path_count;
    }
  }
  Mint ans = 0;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      ans += dp[i][j];
    }
  }
  return ans.val();
}

int main() {
  vector<vector<int>> g = {{1}, {2}};
  out(countPaths(g));
}