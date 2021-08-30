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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1) {
    // Linear Sieve
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
      }
    }
  }

  inline bool is_prime(int n) const { return spf[n] == n; }

  auto factorize(int n) const {
    assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<i64, int>> res;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res.emplace_back(p, count);
    }
    return res;
  }
} const sieve(10000);

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
  //std::sort(res.begin(), res.end());
  return res;
}

// Range Min/Max Query based on Sparse Table.
// - Initialization: O(n*log(n))
// - Query: O(1)
//
// Usage:
//   SparseTableRMQ rmq(a.size(), [&](int i, int j){ return a[i] < a[j]; });
//   auto minval = a[rmq.fold(l, r)];
template<class BetterOp>
struct SparseTableRMQ {
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);

  int n_;                 // sequence size
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<std::vector<int>> sparse_table_;

  SparseTableRMQ(int n, BetterOp better)
      : n_(n),
        better_than_(std::move(better)),
        sparse_table_(n == 0 ? 1 : (msb_log(n) + 1), std::vector<int>(n)) {
    for (int j = 0; j < n; ++j) {
      sparse_table_[0][j] = j;
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < n; ++j) {
        sparse_table_[i + 1][j] =
            better_index(sparse_table_[i][j],
                         sparse_table_[i][std::min(j + (1 << i), n - 1)]);
      }
    }
  }

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    assert(0 <= l and l < r);
    assert(1 <= r and r <= n_);
    const int k = msb_log(r - l);
    const int l2 = r - (1 << k);
    return better_index(sparse_table_[k][l], sparse_table_[k][l2]);
  }

 private:
  // Log base 2 of the most significant bit that is set to 1.
  static inline int msb_log(unsigned x) {
    return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
  }

  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n = in, Q = in;
  vector<int> a(n);
  cin >> a;
  vector<int> R(n, n);
  map<int, int> last_index;
  for (int i = n - 1; i >= 0; --i) {
    auto fac = sieve.factorize(a[i]);
    auto ds = enumerate_divisors(fac);
    int j = n;
    for (auto d: ds) {
      if (d == 1) continue;
      auto it = last_index.find(d);
      if (it != last_index.end()) {
        chmin(j, it->second);
      }
    }
    R[i] = j;
    for (auto d: ds) {
      if (d == 1) continue;
      last_index[d] = i;
    }
  }
  DUMP(R);

  SparseTableRMQ rmq(R.size(), [&](int i, int j) { return R[i] < R[j]; });

  REP(qi, Q) {
    int l = in, r = in;
    --l;
    auto minr = R[rmq.fold(l, r)];
    print((minr < r) ? "No" : "Yes");
  }
}
