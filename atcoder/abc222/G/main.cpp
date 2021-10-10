#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

struct VersatileInput {
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

unordered_map<i64, vector<pair<i64, int>>> cache;

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
const std::vector<std::pair<i64, int>> &factorize(i64 n) {
  const i64 key = n;
  if (auto it = cache.find(key); it != cache.end()) {
    return it->second;
  }
  std::vector<std::pair<i64, int>> res;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    int count = 0;
    do {
      n /= k;
      ++count;
    } while (n % k == 0);
    res.emplace_back(k, count);
  }
  if (n > 1) {
    res.emplace_back(n, 1);
  }
  return (cache[key] = res);
}

i64 totient(i64 n) {
  i64 res = n;
  for (auto[p, _]: factorize(n)) {
    res /= p;
    res *= p - 1;
  }
  return res;
}

// Enumerates divisors from prime factorization.
// O(d(n)) + sorting
std::vector<i64> enumerate_divisors(
    const std::vector<std::pair<i64, int>> &fac) {
  std::vector<i64> res = {1};
  for (auto[p, k]: fac) {
    int sz = res.size();
    for (int i = 0; i < sz; ++i) {
      i64 pp = 1;
      for (int j = 0; j < k; ++j) {
        pp *= p;
        res.push_back(res[i] * pp);
      }
    }
  }
  std::sort(res.begin(), res.end());
  return res;
}

// On a large N, often faster than simple `divisors()`.
std::vector<i64> divisors2(i64 n) { return enumerate_divisors(factorize(n)); }

#include <atcoder/math>

auto solve() -> i64 {
  i64 K = in;
  if (K % 2 == 0) K /= 2;
  K *= 9;
  if (gcd(10, K) != 1) return -1;
  i64 phi = totient(K);
  for (auto d: divisors2(phi)) {
    if (atcoder::pow_mod(10, d, K) == 1) {
      return d;
    }
  }
  return phi;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cache.reserve(1 << 20);
  cache.max_load_factor(0.25);

  const int t = in;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
