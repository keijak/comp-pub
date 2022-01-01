#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

int solve(int H, int W, int K) {
  const int HW = H * W;

  auto shift_right = [&](vector<int> &g) {
    REP(i, H) {
      int tmp = g[i * W];
      for (int j = 1; j < W; ++j) {
        g[i * W + j - 1] = g[i * W + j];
      }
      g[i * W + W - 1] = tmp;
    }
  };
  auto shift_down = [&](vector<int> &g) {
    REP(i, W) {
      int tmp = g[0 * W + i];
      for (int j = 1; j < H; ++j) {
        g[(j - 1) * W + i] = g[j * W + i];
      }
      g[(H - 1) * W + i] = tmp;
    }
  };

  vector<int> coloring(HW);
  set<vector<int>> cs;
  auto f = [&](auto &f, int i) -> void {
    if (i == HW) {
      set<int> uc(ALL(coloring));
      if (int(uc.size()) != K) return;
      vector<int> tmp = coloring;
      vector<int> norm = coloring;
      REP(j, H) {
        REP(k, W) {
          shift_right(tmp);
          chmin(norm, tmp);
        }
        shift_down(tmp);
        chmin(norm, tmp);
      }
      cs.insert(norm);
      return;
    }
    REP(c, K) {
      coloring[i] = c;
      f(f, i + 1);
    }
  };
  f(f, 0);
  return cs.size();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
//  const int H = in, W = in, K = in;
//  cout << solve(H, W, K) << endl;

  for (int h = 1; h <= 10; ++h) {
    cout << '{';
    for (int w = 1; w <= 10; ++w) {
      cout << '{';
      if (h * w <= 10) {
        for (int k = 1; k <= 3; ++k) {
          cout << solve(h, w, k) << (k == 3 ? "" : ",");
        }
      }
      cout << '}' << (w == 10 ? '\n' : ',');
    }
    cout << '}' << (h == 10 ? '\n' : ',');
  }
}
