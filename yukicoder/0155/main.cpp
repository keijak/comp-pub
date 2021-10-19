#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
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

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

template<typename T>
T factorial(int x) {
  static std::vector<T> facts = {1, 1, 2};
  facts.reserve(x + 1);
  while ((int) facts.size() <= x) {
    facts.push_back(facts.back() * T(facts.size()));
  }
  return facts[x];
}

auto solve() -> Real {
  int n = in, L = in;
  L *= 60;
  DUMP(L);
  vector<int> S(n);
  Int ssum = 0;
  REP(i, n) {
    int smin, ssec;
    check(scanf("%d:%d", &smin, &ssec));
    DUMP(i, smin, ssec);
    S[i] = 60 * smin + ssec;
    ssum += S[i];
  }
  if (L >= ssum) { return n; }
  DUMP(S);

  const int T = 18000 + 4000;
  auto dp = vector(n + 1, vector(T, Real(0)));
  dp[0][0] = 1;

  REP(i, n) {
    for (int j = i; j >= 0; --j) {
      for (int k = T - 1; k >= 0; --k) {
        if (j + 1 <= n and k + S[i] < T) {
          dp[j + 1][k + S[i]] += dp[j][k];
        }
      }
    }
  }

  Real ifactn = Real(1) / factorial<Real>(n);
  Real ans = 0;
  REP(i, n) {
    REP(j, n + 1) {
      REP(k, T) {
        if (j + 1 <= n and k + S[i] < T) {
          dp[j + 1][k + S[i]] -= dp[j][k];
        }
      }
    }
    REP(j, n) {
      REP(k, L) {
        if (dp[j][k] == 0) continue;
        Real p = factorial<Real>(j) * factorial<Real>(n - j - 1) * dp[j][k];
        ans += p;
      }
    }
    for (int j = n; j >= 0; --j) {
      for (int k = T - 1; k >= 0; --k) {
        if (j + 1 <= n and k + S[i] < T) {
          dp[j + 1][k + S[i]] += dp[j][k];
        }
      }
    }
  }
  ans *= ifactn;
  return ans;
}

int main() {
//  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
