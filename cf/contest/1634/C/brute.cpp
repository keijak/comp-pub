#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = b), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = b), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}
template <typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template <typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <typename T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &seq, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template <typename T>
    operator T() const {
      T xs(n);
      for (auto &x : xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#endif

using namespace std;

optional<vector<vector<int>>> search(int n, int k) {
  int nk = n * k;
  vector g(n, vector(k, 0));
  vector<int> p(nk);
  REP(i, nk) p[i] = i + 1;
  sort(ALL(p));
  do {
    auto check = [&]() -> bool {
      for (int i = 0; i < n; ++i) {
        vector<Int> cum(k + 1, 0);
        for (int j = 0; j < k; ++j) {
          g[i][j] = p[i * k + j];
          cum[j + 1] = cum[j] + g[i][j];
        }
        for (int w = 2; w <= k; ++w) {
          for (int j = w; j <= k; ++j) {
            Int s = cum[j] - cum[j - w];
            if (s % w != 0) {
              return false;
            }
          }
        }
      }
      return true;
    };
    if (check()) return g;
  } while (next_permutation(ALL(p)));
  return nullopt;
}

int main() {
  //  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  for (int n = 3; n <= 3; ++n) {
    for (int k = 1; k <= 6; ++k) {
      //   cerr << "? " << n << " " << k << endl;
      auto ans = search(n, k);
      if (ans.has_value()) {
        cout << "# " << n << " " << k << endl;
        for (auto row : *ans) print_seq(row);
      } else {
        cout << "! " << n << " " << k << endl;
      }
    }
  }
}
