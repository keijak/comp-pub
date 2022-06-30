// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
const unsigned MOD = 998244353;
using Mint = ModInt<MOD>;

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

auto solve() -> Mint {
  int n = in, s = in;
  vector<pair<int, int>> ab(n);
  REP(i, n) {
    int x = int(in);
    ab[i].first = x - 1;
  }
  REP(i, n) {
    int x = int(in);
    ab[i].second = (x == -1) ? x : (x - 1);
  }
  sort(ALL(ab));

  set<int> missing;
  REP(i, n) missing.insert(i);
  REP(i, n) {
    int b = ab[i].second;
    if (b >= 0) {
      missing.erase(b);
    }
  }
  DUMP(missing);
  DUMP(ab);
  set<int> fixed;
  set<int> miss_pos;
  REP(i, min(s + 1, n)) {
    int b = ab[i].second;
    if (b == -1) {
      miss_pos.insert(i);
    } else {
      fixed.insert(b);
    }
  }
  DUMP(fixed, miss_pos);
  Mint ans = 1;
  REP(i, n) {
    auto [a, b] = ab[i];
    assert(a == i);
    if (missing.count(a)) {
      if (miss_pos.empty()) return 0;
      ans *= ssize(miss_pos);
      miss_pos.erase(miss_pos.begin());
    } else {
      if (not fixed.count(a)) {
        return 0;
      }
      fixed.erase(a);
    }
    if (i + s + 1 < n) {
      int nb = ab[i + s + 1].second;
      if (nb == -1) {
        miss_pos.insert(i + s + 1);
      } else {
        fixed.insert(nb);
      }
    }
  }
  return ans;
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
