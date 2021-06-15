#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
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

// mod: prime
template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (nCk)
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

auto solve() {
  vector<int> freq(26);
  cin >> freq;
  const i64 n = accumulate(ALL(freq), 0LL);
  Factorials fs(n + 5);
  DUMP(freq);

  auto dp = vector(27, vector(n + 10, Mint(0)));
  dp[0][0] = 1;
  i64 placed = 0;
  REP(i, 26) {
    int x = freq[i];
    char ch = 'a' + i;
    // DUMP(ch, x, placed);
    vector<int> choose(x + placed, 0);
    REP(j, x) choose[j] = 1;
    reverse(ALL(choose));
    do {
      int count1 = 0;
      REP(j, 1, choose.size()) {
        if (choose[j] == 1 and choose[j - 1] == 1) {
          ++count1;
        }
      }
      int reduce = 0;
      int count0 = 0;
      dp[i + 1][count1] += dp[i][0];
      REP(j, choose.size()) {
        if (choose[j] == 0) {
          ++count0;
          if (count0 >= 2) {
            if (j > 0 and choose[j - 1] == 1) {
              ++reduce;
            }
            int q = count1 + count0 - 1 - reduce;
            if (q >= 0 and q <= n) {
              dp[i + 1][q] += dp[i][count0 - 1];
            }
          }
        }
      }
      // DUMP(choose, count1, count0, reduce);

    } while (next_permutation(ALL(choose)));
    placed += x;
    // DUMP(dp[i + 1]);
  }
  return dp[n][0];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
