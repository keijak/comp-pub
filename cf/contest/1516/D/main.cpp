#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
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

// Quick factorization.
std::vector<std::pair<int, int>> quick_factorize(int n,
                                                 const std::vector<int> &spf) {
  assert(0 < n and n < int(spf.size()));
  std::vector<std::pair<int, int>> res;
  while (n > 1) {
    int p = spf[n];
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace_back(p, count);
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto spf = sieve_smallest_prime_factors(100005);

  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  cin >> a;
  vector<int> rmin(n + 1, n);
  {
    vector<int> lindex(100005, n);
    for (int i = n - 1; i >= 0; --i) {
      auto fac = quick_factorize(a[i], spf);
      int r = n;
      for (const auto &[p, k] : fac) {
        chmin(r, lindex[p]);
        lindex[p] = i;
      }
      rmin[i] = min(rmin[i + 1], r);
    }
  }

  const int M = 21;
  vector nex(M, vector(n, n));
  REP(i, n) { nex[0][i] = rmin[i]; }
  for (int k = 0; k + 1 < M; ++k) {
    for (int v = 0; v < n; ++v) {
      if (nex[k][v] == n) {
        nex[k + 1][v] = n;
      } else {
        nex[k + 1][v] = nex[k][nex[k][v]];
      }
    }
  }

  REP(qi, q) {
    int l, r;
    cin >> l >> r;
    --l;
    if (nex[0][l] >= r) {
      cout << "1\n";
      continue;
    }
    int fmax = 0;
    int cur = l;
    for (int k = M - 1; k >= 0; --k) {
      int v = nex[k][cur];
      if (v < r) {
        fmax |= (1 << k);
        cur = v;
      }
    }
    cout << fmax + 1 << "\n";
  }
}
