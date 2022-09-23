#define NDEBUG
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

template<typename T>
inline void read_unsigned(T &ret) {
  ret = 0;
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
}

template<typename T>
struct Array2d {
  std::vector<T> data_;
  size_t h_, w_;

  Array2d(size_t h, size_t w) : data_(h * w, -kBig<T>), h_(h), w_(w) {}

  inline T &get(size_t i0, size_t i1) { return data_[i0 * w_ + i1]; }

  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * w_, data_.begin() + (i0 + 1) * w_, val);
  }
};

auto solve() {
  int n, m, K;
  read_unsigned(n);
  read_unsigned(m);
  read_unsigned(K);
  vector<vector<int>> edges(n + 1);

  int a, b;
  REP(i, m) {
    read_unsigned(a);
    read_unsigned(b);
    --a, --b;
    edges[a].push_back(b);
  }
  REP(i, n + 1) sort(ALL(edges[i]));

  Array2d<int> dp(n + 1, K + 1);
  dp.get(0, 0) = 0;
  REP(r, 1, n + 1) {
    REP(k, 1, K + 1) {
      int score = 0;
      int res = -kBig<int>;
      for (int i = r - 1; i >= 0; --i) {
        score += lower_bound(ALL(edges[i]), r) - edges[i].begin();
        chmax(res, dp.get(i, k - 1) + score);
      }
      dp.get(r, k) = res;
    }
  }
  out(dp.get(n, K));
}

int main() {
  init_();
  solve();
  exit_();
}
