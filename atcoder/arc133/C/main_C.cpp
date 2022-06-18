// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() -> Int {
  int H = in, W = in, K = in;
  Mint::set_mod(K);
  vector<int> A = in(H), B = in(W);
  Mint row_sum = Mint(K - 1) * (W - 1);
  Mint col_sum = Mint(K - 1) * (H - 1);
  vector<unsigned> X(H - 1), Y(W - 1);
  Int ans = Int(K - 1) * Int(H - 1) * Int(W - 1);
  Mint last_col = 0, last_row = 0;
  REP(i, H - 1) {
    X[i] = (Mint(A[i]) - row_sum).val();
    last_col += X[i];
    ans += X[i];
  }
  REP(j, W - 1) {
    Y[j] = (Mint(B[j]) - col_sum).val();
    last_row += Y[j];
    ans += Y[j];
  }
  Mint Z = A[H - 1] - last_row;
  if ((B[W - 1] - last_col).val() != Z.val()) {
    return -1;
  }
  ans += Z.val();
  sort(ALL(X));
  sort(ALL(Y));
  while (not X.empty() and not Y.empty()) {
    if (X.back() == Y.back()) {
      Int val = X.back();
      X.pop_back();
      Y.pop_back();
      Int delta = K - 1 - val;
      Mint newz = Z - Mint(delta);
      ans += delta;
      ans -= Z.val();
      ans += newz.val();
      Z = newz;
    } else if (X.back() > Y.back()) {
      X.pop_back();
    } else {
      Y.pop_back();
    }
  }

  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    print(solve());
  }
  exit_();
}
