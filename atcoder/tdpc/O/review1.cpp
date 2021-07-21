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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// mod: prime
template<class T = Mint>
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

Mint count_permutations_no_neighbors_from_same_group(const std::vector<int> &freq) {
  const int n = freq.size();
  const i64 max_slots = std::accumulate(ALL(freq), 0LL) + 1;

  Factorials fs(max_slots);
  auto dp = vector(n + 1, vector(max_slots + 1, Mint(0)));
  int slots = freq[0] + 1;  // insertion points
  dp[1][slots - 2] = 1;  // exclude left/right end points.
  for (int i = 1; i < n; ++i) {
    // j slots are violating the no-neighbor-from-same-group rule.
    for (int j = 0; j <= slots - 2; ++j) {
      if (dp[i][j].val() == 0) continue;
      int slots_ok = slots - j;
      // Break `freq[i]` items into `p` buckets.
      for (int p = 1; p <= freq[i]; ++p) {
        auto x = fs.C(freq[i] - 1, p - 1);
        for (int q = 0; q <= min(j, p); ++q) {
          int nj = j - q + freq[i] - p;
          if (nj <= max_slots) {
            dp[i + 1][nj] += dp[i][j] * x * fs.C(j, q) * fs.C(slots_ok, p - q);
          }
        }
      }
    }
    slots += freq[i];
  }
  return dp[n][0];
}

auto solve() {
  vector<int> freq;
  REP(i, 26) {
    INPUT(int, f);
    if (f == 0) continue;
    freq.push_back(f);
  }
  return count_permutations_no_neighbors_from_same_group(freq);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
