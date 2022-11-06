// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/convolution>
#include <atcoder/math>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = b), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = b), true);
}
template <typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}
#endif

template <size_t BufSize>
class InputReader {
 public:
  void skip() {
    // Initialize on the first use.
    [[maybe_unused]] static const bool init = [this]() {
      const size_t len = fread(buf, 1, sizeof(buf) - 1, stdin);
      buf[len] = '\0';
      p = buf;
      bufend = buf + len;
      return true;
    }();

    while (isspace(*p)) ++p;
  }

  template <typename T>
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
    template <typename T>
    operator T() const {
      T xs(n);
      for (auto &x : xs) {
        reader.skip();
        assert(not reader.is_eof());
        reader.read_one(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) { return {*this, n}; }

  bool is_eof() { return p >= bufend; }

 private:
  static inline char buf[BufSize];
  char *p, *bufend;

  template <class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*p == '-') ? (++p, -1) : 1;
    }
    x = 0;
    while (isdigit(*p)) x = x * 10 + (*p++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = p;
    while (not isspace(*p)) p++;
    s.assign(p0, p);
  }
  void read_one(std::string_view &s) {
    const char *p0 = p;
    while (not isspace(*p)) p++;
    s = std::string_view(p0, p - p0);
  }
};
InputReader<(1 << 26)> in;

template <typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template <typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
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
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
// #define cerr if(false)cerr
#endif

using namespace std;

Mint solve_slow(int N, int M) {
  auto dp = vector(N, vector(M + 1, Mint(0)));
  REP(i, M + 1) dp[0][i] = 1;
  REP(i, N - 1) {
    REP(j, M + 1) {
      for (int k = 0;; ++k) {
        if (j + 3 * k > M) break;
        REP(d, 1, 3) {
          int r = j + 3 * k + d;
          if (r > M) break;
          dp[i + 1][r] += dp[i][j];
        }
      }
    }
  }
  return accumulate(ALL(dp[N - 1]), Mint(0));
}

auto solve() {
  const int N = in, M = in;
  out(solve_slow(N, M));
}

int main() {
  init_();
  const int T = 1;  // in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
