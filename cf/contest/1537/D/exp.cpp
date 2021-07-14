#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

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

std::vector<i64> divisors(i64 n) {
  std::vector<i64> res;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    i64 q = n / k;
    if (q != k) res.push_back(q);
  }
  // std::sort(res.begin(), res.end());
  return res;
}

const int L = 200000;

char wint[L];
gp_hash_table<i64, bool> memo;

bool win(int n) {
  if (n < L) {
    return wint[n];
  }
  bool is_prime = true;
  bool wins = false;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    if (not win(n - k)) {
      wins = true;
      break;
    }
    i64 q = n / k;
    if (not win(n - q)) {
      wins = true;
      break;
    }
  }
  return wins;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  memo.resize(1000000);

  REP(i, 1, L) {
    for (auto d : divisors(i)) {
      if (not wint[i - d]) {
        wint[i] = true;
        break;
      }
    }
  }

  REP(i, 60) {
    cout << i << "|\t";
    REP(j, 10) {
      int x = 10 * i + j;
      cout << win(x) << "\t";
    }
    cout << endl;
  }

  // int t;
  // cin >> t;
  // REP(i, t) {
  //   int n;
  //   cin >> n;
  //   cout << (win(n) ? "Alice" : "Bob") << "\n";
  // }
}
