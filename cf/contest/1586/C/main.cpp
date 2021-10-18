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
    return std::cout << (x ? "YES" : "NO") << endc;
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

struct Cell {
  bool ud;
  bool l;
  bool r;
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, m = in;
  vector<string> g(n);
  REP(i, n) { cin >> g[i]; }

  auto cells = vector(m, vector(n, Cell{0, 0, 0}));
  auto cc = vector(m + 1, vector(n, 0));
  auto rc = vector(m, vector(n, 0));
  REP(j, m) {
    REP(i, n) {
      bool u = (i > 0 and g[i - 1][j] == 'X');
      bool d = (i == n - 1 or g[i + 1][j] == 'X');
      bool l = (j > 0 and g[i][j - 1] == 'X');
      bool r = (j == m - 1 or g[i][j + 1] == 'X');
      cells[j][i] = Cell{(u and d), l, r};
    }
  }
  REP(j, m) {
    REP(i, n) {
      int c = (cells[j][i].ud and cells[j][i].l and cells[j][i].r) ? 1 : 0;
      cc[j + 1][i] = cc[j][i] + c;
      rc[j][i] = (cells[j][i].ud and cells[j][i].l) ? 1 : 0;
    }
  }
  REP(i, n) {
    REP(j, m) {
      cerr << (cc[j + 1][i] - cc[j][i]) << " ";
    }
    cerr << endl;
  }
  cerr << endl;

  int Q = in;
  REP(qi, Q) {
    int l = in, r = in;
    --l;
    if (r - l == 1) {
      print(true);
      continue;
    }
    int cnt = 0;
    REP(i, n) {
      cnt += cc[r - 1][i] - cc[l + 1][i];
      cnt += rc[r - 1][i];
    }
    print(cnt == 0);
  }
}
