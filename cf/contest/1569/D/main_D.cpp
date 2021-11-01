#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

auto solve() {
  int n = in, m = in, K = in;
  vector<Int> X = in(n), Y = in(m);
  Compressed<Int> cx(X), cy(Y);
  DUMP(cx.values);
  DUMP(cy.values);
  deque<pair<int, int>> qv, qh;
  map<Int, Int> tv, th;
  vector<map<Int, Int>> pv(cx.size()), ph(cy.size());
  pv.emplace_back();
  ph.emplace_back();

  REP(i, K) {
    Int x = in, y = in;

    bool ver = false, hor = false;
    if (binary_search(ALL(cx.values), x)) {
      ver = true;
    }
    if (binary_search(ALL(cy.values), y)) {
      hor = true;
    }
    int xi = cx.index(x);
    int yi = cy.index(y);
    if (ver and hor) {
      //
    } else if (ver) {
      tv[yi]++;
      pv[xi][yi]++;
      qv.emplace_back(xi, yi);
    } else if (hor) {
      th[xi]++;
      ph[yi][xi]++;
      qh.emplace_back(yi, xi);
    } else {
      check(false);
    }
  }
  sort(ALL(qv));
  sort(ALL(qh));
  Int ans = 0;
  REP(xi, cx.size()) {
    map<Int, Int> mc;
    while (not qv.empty() and qv.front().first == xi) {
      int yi = qv.front().second;
      qv.pop_front();
      mc[yi]++;
    }
    for (auto[yi, cnt]: mc) {
      ans += (tv[yi] - cnt) * cnt;
      tv[yi] -= cnt;
    }
  }
  REP(yi, cy.size()) {
    map<Int, Int> mc;
    while (not qh.empty() and qh.front().first == yi) {
      int xi = qh.front().second;
      qh.pop_front();
      mc[xi]++;
    }
    for (auto[xi, cnt]: mc) {
      ans += (th[xi] - cnt) * cnt;
      th[xi] -= cnt;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
