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

  bool is_eof() { return ptr_ >= bufend_; }

 private:
  static inline char buf_[BufSize];
  char *ptr_, *bufend_;

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
  int n = in;
  vector<int> A(n), B(n);
  vector<int> vs;
  REP(i, n) {
    int a = in, b = in;
    A[i] = a;
    B[i] = b;
    vs.push_back(a);
    vs.push_back(b);
  }
  vs.push_back(1);
  Compressed<int> vc(vs);
  vector<vector<int>> g(vc.size());
  REP(i, n) {
    int ai = vc.index(A[i]);
    int bi = vc.index(B[i]);
    g[ai].push_back(bi);
    g[bi].push_back(ai);
  }
  queue<int> q;
  q.push(vc.index(1));
  vector<int> done(vc.size());
  done[vc.index(1)] = 1;
  int ans = 1;
  while (not q.empty()) {
    int x = q.front();
    q.pop();
    chmax(ans, vc.value(x));
    for (int u: g[x]) {
      if (done[u]) continue;
      done[u] = 1;
      q.push(u);
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
