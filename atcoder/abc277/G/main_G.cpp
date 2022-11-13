// #define NDEBUG
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <string_view>
#include <iostream>
#include <cstdio>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

template<size_t BufSize>
class InputReader {
 public:
  void skip() {
    // Initialize on the first use.
    [[maybe_unused]] static const bool init = [this]() {
      const size_t len = fread(buf_, 1, sizeof(buf_) - 1, stdin);
      buf_[len] = '\0';
      ptr_ = buf_;
      bufend_ = buf_ + len;
      return true;
    }();
    while (isspace(*ptr_)) ++ptr_;
  }

  template<typename T>
  operator T() {
    T x;
    skip();
    assert(not is_eof());  // Couldn't read reached the end of input.
    read_one(x);
    return x;
  }

  struct Sized {
    InputReader<BufSize> &reader;
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        reader.skip();
        assert(not reader.is_eof());
        reader.read_one(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) { return {*this, n}; }

  bool is_eof() const { return ptr_ >= bufend_; }

 private:
  template<class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*ptr_ == '-') ? (++ptr_, -1) : 1;
    }
    x = 0;
    while (isdigit(*ptr_)) x = x * 10 + (*ptr_++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s.assign(p0, ptr_);
  }
  void read_one(std::string_view &s) {
    const char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s = std::string_view(p0, ptr_ - p0);
  }

  static inline char buf_[BufSize];
  char *ptr_, *bufend_;
};
InputReader<(1 << 26)> in;

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

void init_() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
  //std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  in.skip();
  assert(in.is_eof());  // Some input is left.
#endif
  fflush(stdout), fflush(stderr);
  std::cout.flush(), std::cerr.flush();
  std::_Exit(0);
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

template<typename T>
struct Array4d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0;

  Array4d(size_t d0, size_t d1, size_t d2, size_t d3)
      : data_(d0 * d1 * d2 * d3, T{}),
        b0_(d1 * d2 * d3),
        b1_(d2 * d3),
        b2_(d3) {}

  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

using namespace std;

auto solve() {
  const int N = in, M = in, K = in;
  vector<vector<int>> g(N);
  REP(i, M) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> C = in(N);
  vector<Mint> invdeg(N);
  REP(v, N) {
    invdeg[v] = Mint(ssize(g[v])).inv();
  }
  Array4d<Mint> dp(2, N + 1, 2, 2);
  dp.get(0, 0, 0, 0) = 1;
  Mint ans = 0;
  REP(i, 1, K + 1) {
    const int i1 = i & 1;
    const int i0 = i1 ^ 1;
    dp.fill(i1, Mint(0));
    REP(v, N) {
      const bool level_up = C[v] == 0;
      for (int u: g[v]) {
        Mint p = invdeg[u];
        REP(x, 2) {
          REP(y, 2) {
            Mint co = dp.get(i0, u, x, y);
            if (level_up) {
              if (x) co += dp.get(i0, u, 0, y);
              if (y) {
                co += dp.get(i0, u, x, 0);
                if (x) co += dp.get(i0, u, 0, 0);
              }
            }
            dp.get(i1, v, x, y) += co * p;
          }
        }
      }
      if (not level_up) {
        ans += dp.get(i1, v, 1, 1);
      }
    }
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
