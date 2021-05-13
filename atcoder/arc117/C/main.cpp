#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::static_modint<3>;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <class T = Mint>
struct FactorialsSmallMod {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;
  std::vector<int> mcount;  // How many MOD factors the factorial includes.

  // n: max cached value. can be greater than the mod.
  FactorialsSmallMod(int n) : fact(n + 1), ifact(n + 1), mcount(n + 1) {
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      int x = i;
      int count = 0;
      while (x % Mint::mod() == 0) {
        ++count;
        x /= Mint::mod();
      }
      fact[i] = fact[i - 1] * x;
      mcount[i] = mcount[i - 1] + count;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      int x = i;
      while (x % Mint::mod() == 0) {
        x /= Mint::mod();
      }
      ifact[i - 1] = ifact[i] * x;
    }
  }

  // Combination (nCk)
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    int p = mcount[n] - mcount[k] - mcount[n - k];
    if (p > 0) return 0;
    return fact[n] * ifact[k] * fact[n - k];
  }
};

char solve() {
  map<char, int> dec;
  dec['B'] = 0;
  dec['W'] = 1;
  dec['R'] = 2;
  array<char, 3> enc;
  enc[0] = 'B';
  enc[1] = 'W';
  enc[2] = 'R';

  int n;
  cin >> n;
  vector<int> a(n);
  REP(i, n) {
    char ch;
    cin >> ch;
    a[i] = dec[ch];
  }
  FactorialsSmallMod<Mint> fs(n);
  Mint c = 0;
  REP(i, n) { c += fs.C(n - 1, i) * a[i]; }
  if (n % 2 == 0) c *= -1;
  return enc[c.val()];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
