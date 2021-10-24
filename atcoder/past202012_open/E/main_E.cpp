#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

template<typename V>
void rotate90(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  std::vector<V> tmp(w, V(h, grid[0][0]));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      tmp[w - 1 - j][i] = grid[i][j];
    }
  }
  std::swap(grid, tmp);
}

auto solve() {
  int H = in, W = in;
  vector<string> S(H), T(H);
  REP(i, H) { S[i] = string(in); }
  REP(i, H) { T[i] = string(in); }
  DUMP(S);
  DUMP(T);
  REP(rot, 4) {
    int li = 20, lj = 20, ri = -1, rj = -1;
    REP(i, ssize(T)) {
      REP(j, ssize(T[i])) {
        if (T[i][j] == '#') {
          chmin(li, i);
          chmin(lj, j);
          chmax(ri, i);
          chmax(rj, j);
        }
      }
    }
    check(li != 20);
    auto tt = vector(ri - li + 1, string(rj - lj + 1, '.'));
    for (int i = li; i <= ri; ++i) {
      for (int j = lj; j <= rj; ++j) {
        tt[i - li][j - lj] = T[i][j];
      }
    }
    int h = ssize(tt);
    int w = ssize(tt[0]);

//    cerr << "----" << endl;
//    REP(i, h) {
//      DUMP(tt[i]);
//    }
//    cerr << "====" << endl;

    for (int i = 0; i + h <= H; ++i) {
      for (int j = 0; j + w <= W; ++j) {
        REP(x, h) {
          REP(y, w) {
            if (tt[x][y] == '#' and S[i + x][j + y] == '#') {
              goto bad;
            }
          }
        }
        return true;
        bad:;
      }
    }
    rotate90(T);
  }
  return false;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
