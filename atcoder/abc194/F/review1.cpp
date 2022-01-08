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

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class T = Mint>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}
int get_digit(char ch) {
  if ('0' <= ch and ch <= '9') {
    return ch - '0';
  }
  return (ch - 'A') + 10;
}

auto solve() -> Mint {
  const string N = in;
  const int K = in;
  const int n = ssize(N);
  auto C = comb_table(20);

  if (n == 1) {
    return (K == 1) ? get_digit(N[0]) : 0;
  }

  auto memo = vector(n + 1, vector(K + 1, vector(2, optional<Mint>())));

  auto f = [&](auto &rec, int m, int u, bool zreq) -> Mint {
    if (u > K) return 0;
    if (m == 0) {
      return (u == K) ? 1 : 0;
    }
    if (memo[m][u][zreq].has_value()) {
      return *memo[m][u][zreq];
    }
    Mint ret = 0;
    if (u + 1 <= K) {
      if (u == 0) {
        ret += rec(rec, m - 1, 1, zreq) * (zreq ? (K - 1) : K);
      } else {
        ret += rec(rec, m - 1, u + 1, zreq) * (K - u);
      }
    }
    if (u == 0) {
      ret += rec(rec, m - 1, u, zreq);
    } else {
      ret += rec(rec, m - 1, u, zreq) * u;
    }
    memo[m][u][zreq] = ret;
    return ret;
  };

  Mint ans = 0;
  set<int> seen;
  REP(i, n) {
    int cur_d = get_digit(N[i]);
    for (int j = 0; j < cur_d; ++j) {
      int u = ssize(seen);
      if (not seen.count(j) and not(seen.empty() and j == 0)) ++u;
      if (u > K) continue;
      bool zused = seen.count(0);
      if (zused) {
        auto sub = f(f, n - i - 1, u, false);
        Mint c1 = C[16 - u][K - u];
        ans += sub * c1;
      } else {
        if (15 - u >= 0 and K - 1 - u >= 0) {
          auto sub0 = f(f, n - i - 1, u, true);
          Mint c0 = C[15 - u][K - 1 - u];
          ans += sub0 * c0;
        }
        auto sub1 = f(f, n - i - 1, u, false);
        Mint c1 = C[15 - u][K - u];
        ans += sub1 * c1;
      }
    }
    seen.insert(cur_d);
  }

  set<char> chars(ALL(N));
  if (ssize(chars) == K) {
    ++ans;
    DUMP("extra");
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
