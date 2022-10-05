// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
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

auto solve(int N, int K) {
  if (K > N / 2) {
    out(-1);
    return;
  }
  vector<int> A(N);
  set<int> S;
  REP(i, N) {
    S.insert(1 + i);
  }
  for (int i = N; i > N - K; --i) {
    A[i - K - 1] = i;
    S.erase(i);
  }
  REP(i, N) {
    if (A[i] != 0) continue;
    assert(not S.empty());
    int mini = *S.begin();
    if (mini <= i + 1 - K) {
      A[i] = mini;
      S.erase(S.begin());
      continue;
    }
    auto it = S.lower_bound(i + 1 + K);
    if (it == S.end()) {
      DUMP(N, K, S, i);
    }
    A[i] = *it;
    S.erase(it);
  }
  out_seq(A);
}
//
//void run_exp() {
//  for (int n = 2; n <= 11; ++n) {
//    for (int k = 1; k <= n - 1; ++k) {
//      vector<int> v(n);
//      REP(i, n) v[i] = i + 1;
//      vector<int> ans(n, 100);
//      do {
//        bool ok = [&]() {
//          REP(i, n) if (abs(v[i] - (i + 1)) < k) {
//              return false;
//            }
//          return true;
//        }();
//        if (ok) {
//          chmin(ans, v);
//        }
//      } while (next_permutation(ALL(v)));
//      auto z = solve(n, k);
//      if (ans != z) {
//        cerr << "####" << endl;
//        out(n, k);
//        out_seq(ans);
//        out_seq(z);
//      }
//    }
//  }
//}

int main() {
  init_();
  int N = in, K = in;
  solve(N, K);
  exit_();
}
