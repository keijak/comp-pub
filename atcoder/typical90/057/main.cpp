#include <bits/stdc++.h>

#include <atcoder/math>

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

using BitSet = vector<char>;

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<BitSet> bitsets(n);
  REP(i, n) {
    BitSet bs(m, 0);
    int t;
    cin >> t;
    REP(j, t) {
      int a;
      cin >> a;
      --a;
      bs[a] = 1;
    }
    bitsets.push_back(move(bs));
  }
  sort(ALL(bitsets), greater{});

  BitSet S(m);
  REP(i, m) {
    int x;
    cin >> x;
    S[i] = x;
  }

  vector<optional<BitSet>> basis(m);
  int basis_count = 0;
  REP(i, n) {
    if (basis_count == m) break;
    BitSet bs = bitsets[i];
    for (int j = 0; j < m; ++j) {
      if (not bs[j]) continue;
      if (not basis[j]) {
        basis[j] = move(bs);
        ++basis_count;
        break;
      }
      const BitSet &v = *basis[j];
      for (int k = 0; k < m; ++k) {
        bs[k] ^= v[k];
      }
    }
  }

  BitSet T(m);
  REP(i, m) {
    if (S[i] == T[i]) continue;
    if (not basis[i]) return 0;
    const BitSet &v = *basis[i];
    for (int j = i; j < m; ++j) {
      T[j] ^= v[j];
    }
  }
  return atcoder::pow_mod(2, n - basis_count, 998244353);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
