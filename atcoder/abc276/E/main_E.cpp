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

template<size_t BufSize>
class InputReader {
 public:
  InputReader() : p{buf} {
    const size_t len = fread /* _unlocked */ (buf, 1, sizeof(buf) - 1, stdin);
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
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() -> bool {
  int H = in, W = in;
  auto inside = [&](int x, int y) {
    return 0 <= x and x < H and 0 <= y and y < W;
  };
  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    static const int dx[] = {1, 0, -1, 0};
    static const int dy[] = {0, 1, 0, -1};
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      if (not inside(nx, ny)) continue;
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  vector<string> g(H);
  int si = -1, sj = -1;
  REP(i, H) {
    g[i] = string(in);
    assert(ssize(g[i]) == W);
    REP(j, W) {
      if (g[i][j] == 'S') {
        si = i;
        sj = j;
      }
    }
  }
  assert(si != -1 and sj != -1);
  queue<tuple<int, int, int, int>> q;
  auto dis = vector(H, vector(W, -1));
  auto color = vector(H, vector(W, -1));
  int color_counter = 0;
  for (auto [ni, nj]: adjacent(si, sj)) {
    if (g[ni][nj] == '#') continue;
    dis[ni][nj] = 1;
    color[ni][nj] = ++color_counter;
    q.emplace(1, color_counter, ni, nj);
  }

  while (not q.empty()) {
    auto [d, c, i, j] = q.front();
    q.pop();
    for (auto [ni, nj]: adjacent(i, j)) {
      if (g[ni][nj] == '#') continue;
      if (g[ni][nj] == 'S') continue;
      if (dis[ni][nj] == -1) {
        dis[ni][nj] = d + 1;
        color[ni][nj] = c;
        q.emplace(d + 1, c, ni, nj);
      } else if (color[ni][nj] != c and dis[ni][nj] + d + 1 >= 4) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
