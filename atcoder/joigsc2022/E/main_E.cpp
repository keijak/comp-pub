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

template<size_t BufSize>
class StdinReader {
 public:
  StdinReader() : p{buf} {
    const size_t len = fread(buf, 1, sizeof(buf) - 1, stdin);
    buf[len] = '\0';
    bufend = buf + len;
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
    StdinReader<BufSize> &reader;
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

  void skip() {
    while (isspace(*p)) ++p;
  }

  bool is_eof() { return p >= bufend; }

 private:
  static inline char buf[BufSize];
  char *p, *bufend;

  template<class T>
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
StdinReader<(1 << 26)> in;

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

#endif
  std::cout.flush(), std::fflush(stdout), std::_Exit(0);
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

auto solve() {
  const int n = in, L = in;
  const vector<int> A = in(n);
  vector<Int> acc(n + 1);
  REP(i, n) {
    acc[i + 1] = acc[i] + A[i];
  }
  DUMP(n, L);
  DUMP(A);
  vector<int> nex(n), dp(n + 1), dpw(n + 1, 0);
  int r = 0;
  for (int l = 0; l < n; ++l) {
    chmax(r, l);
    while (r <= n and acc[r] <= acc[l] + L) ++r;
    nex[l] = r - 1;
  }
  for (int i = n - 1; i >= 0; --i) {
    dp[i] = dp[nex[i]] + 1;
    if (nex[i] == n) {
      dpw[i] = acc[n] - acc[i];
    } else {
      dpw[i] = dpw[nex[i]];
    }
  }
  REP(i, n) printf("%d %d\n", dp[i], dpw[i]);
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
