#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

struct C {
  bool locked;
  i64 right;
  i64 down;
};
ostream &operator<<(ostream &os, const C &c) {
  return os << "(" << (c.locked ? 'L' : 'F') << ",R=" << c.right << ",D=" << c.down << ")";
}

auto solve() {
  const int n = in, m = in, Q = in;
  auto g = vector(n, vector(m, C{0, 0, 0}));
  i64 s = 0;
  for (int i = n - 1; i >= 0; --i) {
    for (int j = m - 1; j >= 0; --j) {
      s += 1;
      if (j + 1 < m) {
        i64 x = 1 + g[i][j + 1].down;
        s += x;
        g[i][j].right += x;
      }
      if (i + 1 < n) {
        i64 x = 1 + g[i + 1][j].right;
        s += x;
        g[i][j].down += x;
      }
    }
  }
  DUMP(s);

  REP(qi, Q) {
    int x = in, y = in;
    --x, --y;
    bool locking = not g[x][y].locked;
    g[x][y].locked = locking;
    //DUMP(qi, s, x, y, locking);

    if (locking) {
      s -= 1;
      s -= g[x][y].right;
      s -= g[x][y].down;
      g[x][y].right = 0;
      g[x][y].down = 0;
    } else {
      if (y + 1 < m and not g[x][y + 1].locked) {
        s += g[x][y].right = 1 + g[x][y + 1].down;
      }
      if (x + 1 < n and not g[x + 1][y].locked) {
        s += g[x][y].down = 1 + g[x + 1][y].right;
      }
      s += 1;
    }

    bool first = true;
    while (x >= 0 and y >= 0) {
      if (first) {
        first = false;
      } else {
        if (g[x][y].locked) break;
        if (y + 1 < m and not g[x][y + 1].locked) {
          s -= g[x][y].right;
          g[x][y].right = 1 + g[x][y + 1].down;
          s += g[x][y].right;
        }
        if (x + 1 < n and not g[x + 1][y].locked) {
          s -= g[x][y].down;
          g[x][y].down = 1 + g[x + 1][y].right;
          s += g[x][y].down;
        }
      }

      bool changed = false;
      if (x - 1 >= 0 and not g[x - 1][y].locked) {
        s -= g[x - 1][y].down;
        g[x - 1][y].down = g[x][y].locked ? 0 : (1 + g[x][y].right);
        s += g[x - 1][y].down;
        changed = true;
      }
      if (y - 1 >= 0 and not g[x][y - 1].locked) {
        s -= g[x][y - 1].right;
        g[x][y - 1].right = g[x][y].locked ? 0 : (1 + g[x][y].down);
        s += g[x][y - 1].right;
        changed = true;
      }
      if (not changed) break;
      --x, --y;
    }

    print(s);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    solve();
  }
}
