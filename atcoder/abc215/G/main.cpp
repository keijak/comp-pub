#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    return (std::cin >> x), x;
  }
} in;

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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

auto solve() {
  int n = in;
  vector<int> C(n);
  cin >> C;
  Compressed<int> cc(C);
  vector<int> freq(cc.size(), 0);
  REP(i, n) {
    int j = cc.index(C[i]);
    ++freq[j];
  }
  DUMP(freq);
  map<int, int> ff;
  for (auto x : freq) {
    ++ff[x];
  }

  Factorials fs(n);

  for (int k = 1; k <= n; ++k) {
    Mint ans = 0;
    for (auto[f, cnt] : ff) {
      ans += (fs.C(n, k) - fs.C(n - f, k)) * cnt;
    }
    ans /= fs.C(n, k);
    print(ans);
  }

}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    solve();
  }
}
