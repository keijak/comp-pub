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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T>
void sort_unique(std::vector<T> &v) {
  std::sort(v.begin(), v.end());
  v.erase(std::unique(v.begin(), v.end()), v.end());
}

std::vector<i64> divisors(i64 n) {
  std::vector<i64> res;
  for (i64 k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    i64 q = n / k;
    if (q != k) res.push_back(q);
  }
  //std::sort(res.begin(), res.end());
  return res;
}

auto solve() -> i64 {
  const int n = in;
  vector<pair<i64, i64>> ab(n);
  for (auto&[a, b]: ab) cin >> a >> b;
  if (n == 1) {
    return std::lcm(ab[0].first, ab[0].second);
  }
  i64 all_g = gcd(ab[0].first, ab[0].second);
  REP(i, n) {
    auto[a, b] = ab[i];
    all_g = gcd(gcd(all_g, a), b);
  }
  for (auto&[a, b]: ab) {
    assert(a % all_g == 0);
    assert(b % all_g == 0);
    a /= all_g;
    b /= all_g;
  }
  set<i64> ds;
  for (auto[a, b]: ab) {
    auto da = divisors(a);
    ds.insert(ALL(da));
    auto db = divisors(b);
    ds.insert(ALL(db));
  }
  vector<i64> dcand;
  for (auto d: ds) {
    for (const auto&[a, b]: ab) {
      if (a % d != 0 and b % d != 0) {
        goto skip1;
      }
    }
    dcand.push_back(d);
    skip1:;
  }
  i64 ans = 1;
  REP(i, ssize(dcand)) {
    const i64 d1 = dcand[i];
    REP(j, i + 1) {
      const i64 d2 = dcand[j];
      for (const auto&[a, b]: ab) {
        if ((a % d1 == 0 and b % d2 == 0) or (a % d2 == 0 and b % d1 == 0)) {
          // ok
        } else {
          goto loop_end;
        }
      }
      chmax(ans, std::lcm(d1, d2));
      loop_end:;
    }
  }
  return ans * all_g;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
