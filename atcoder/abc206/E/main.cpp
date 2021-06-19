#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

// Returns a table of divisor counts.
std::vector<int> divisor_count_table(int n) {
  std::vector<int> counts(n + 1, 1);
  counts[0] = 0;
  for (int i = 2; i * i <= n; ++i) {
    if (counts[i] != 1) continue;
    for (int j = i * i; j <= n; j += i) {
      ++counts[j];
    }
  }
  return counts;
}

// Returns a bitset to tell if each number is prime. O(n log log n).
std::vector<bool> prime_sieve(int n) {
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i <= n; i += 2) is_prime[i] = false;
  for (int i = 3; i * i <= n; i += 2) {
    if (not is_prime[i]) continue;
    for (int j = i * i; j <= n; j += i) is_prime[j] = false;
  }
  return is_prime;
}

// Returns a vector that maps x to x's smallest prime factor (> 1).
// O(n log log n)
std::vector<int> sieve_smallest_prime_factors(int n) {
  std::vector<int> res(n + 1);
  for (int i = 1; i <= n; ++i) res[i] = i;
  for (int i = 2; i <= n; i += 2) res[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (res[i] != i) continue;
    for (int j = i * i; j <= n; j += i) {
      if (res[j] == j) res[j] = i;
    }
  }
  return res;
}

// Quick factorization.
std::vector<std::pair<int, int>> quick_factorize(int n,
                                                 const std::vector<int> &spf) {
  assert(0 < n and n < int(spf.size()));
  std::vector<std::pair<int, int>> res;
  for (;;) {
    const int p = spf[n];
    if (p == 1) break;
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace_back(p, count);
  }
  return res;
}

const int M = 1'000'005;

auto solve() {
  int L, R;
  cin >> L >> R;
  if (L == 1) ++L;

  auto spf = sieve_smallest_prime_factors(M);
  vector<i64> dd(R + 5), mcount(R + 5);
  vector<vector<int>> divs(R + 5);
  for (int i = 2; i <= R; ++i) {
    auto fac = quick_factorize(i, spf);
    int all_one = true;
    for (auto [p, k] : fac) {
      if (k > 1) {
        all_one = false;
        break;
      }
    }
    if (not all_one) continue;
    int mc = 0;
    for (int j = i; j <= R; j += i) {
      divs[j].push_back(i);
      if (L <= j) {
        mc++;
      }
    }
    dd[i] = fac.size();
    mcount[i] = mc;
  }

  vector<i64> dcount(R + 5, 0);
  for (int x = L; x <= R; ++x) {
    for (int y = x * 2; y <= R; y += x) {
      dcount[y]++;
    }
  }

  i64 ans = 0;
  const i64 tot = R - (L - 1);
  for (i64 x = L; x <= R; ++x) {
    i64 cop = tot;  // coprime
    for (i64 d : divs[x]) {
      i64 k = dd[d];
      if (k % 2 == 1) {
        cop -= mcount[d];
      } else {
        cop += mcount[d];
      }
    }
    i64 nd = dcount[x];
    i64 nm = R / x;
    ans += (tot - cop - nm - nd);
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
