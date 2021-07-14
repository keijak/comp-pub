#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
#include <atcoder/modint>
using Mint = atcoder::modint;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

auto solve() {
  i64 n, P, Q;
  cin >> n >> P >> Q;
  Mint::set_mod(P);
  vector<i64> a(n);
  cin >> a;

  vector<unordered_map<i64, int>> cuml(n), cumr(n);
  REP(i, n) {
    cuml[i].max_load_factor(0.25);
    cumr[i].max_load_factor(0.25);
  }
  {
    unordered_map<i64, int> pp;  // pair product
    for (int i = 1; i + 3 < n; ++i) {
      for (int j = 0; j < i; ++j) {
        i64 x = ((a[i] % P) * (a[j] % P)) % P;
        ++pp[x];
      }
      cuml[i] = pp;
    }
  }
  {
    unordered_map<i64, int> pp;  // pair product
    for (int i = n - 2; i - 3 >= 0; --i) {
      for (int j = i + 1; j < n; ++j) {
        i64 x = ((a[i] % P) * (a[j] % P)) % P;
        ++pp[x];
      }
      cumr[i] = pp;
    }
  }

  int ans = 0;
  for (int mid = 2; mid + 2 < n; ++mid) {
    Mint x = a[mid];
    for (const auto &[b, bc] : cuml[mid - 1]) {
      Mint xb = x * b;
      for (const auto &[c, cc] : cumr[mid + 1]) {
        Mint p5 = xb * c;
        if (p5.val() == Q) {
          ans += bc * cc;
        }
      }
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
