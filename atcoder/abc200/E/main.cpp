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

const int NMAX = 1'000'001;
using Int = __int128_t;

array<i64, 3> solve() {
  i64 n, k;
  cin >> n >> k;
  const i64 n3 = n * 3;

  vector<i64> D(n3 + 1, 0);
  REP(i, n3 + 1) { D[i] = i64(i + 2) * (i + 1) / 2; }

  vector<i64> C(n3 + 1, 0);
  REP(i, n3 + 1) {
    if (i + 3 <= n3) {
      C[i + 3] += D[i];
    }
    if (i + (n + 3) <= n3) {
      C[i + n + 3] -= D[i] * 3;
    }
    if (i + (2 * n + 3) <= n3) {
      C[i + 2 * n + 3] += D[i] * 3;
    }
    if (i + (3 * n + 3) <= n3) {
      C[i + 3 * n + 3] -= D[i];
    }
  }

  const i64 n2 = n * 2;
  vector<i64> B(n2 + 1, 0);
  REP(i, n2 + 1) {
    if (i + 2 <= n2) {
      B[i + 2] += i + 1;
    }
    if (i + (n + 2) <= n2) {
      B[i + n + 2] -= 2 * (i + 1);
    }
    if (i + 2 * n + 2 <= n2) {
      B[i + 2 * n + 2] += i + 1;
    }
  }

  vector<Int> zacc(n3 + 2, 0LL);
  for (i64 z = 3; z <= n3; ++z) {
    // zcount[z] = Int(z - 1) * (z - 2) / 2;
    zacc[z + 1] = zacc[z] + C[z];
  }
  i64 tv = 3, fv = n3 + 1;
  while (fv - tv > 1) {
    i64 mid = (fv + tv) / 2;
    if (zacc[mid] < k) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  const i64 z = tv;
  DUMP(tv, C[tv], (i64)zacc[tv - 1], (i64)zacc[tv], (i64)zacc[tv + 1]);
  i64 rem = k - zacc[tv];
  DUMP(rem);

  for (i64 i = 1; i <= n; ++i) {
    if (i > z - 2) break;
    if (z - i > n2) continue;
    if (rem > B[z - i]) {
      rem -= B[z - i];
      continue;
    }
    for (i64 j = max(z - i - n, 1LL); j <= n; ++j) {
      i64 p = z - i - j;
      if (p <= 0) break;
      if (p > n) continue;
      if (--rem == 0) {
        return {i, j, p};
      }
    }
  }
  assert(false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  print_seq(solve());
}
