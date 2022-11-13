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

optional<vector<int>> topological_sort(const vector<vector<int>> &g) {
  const int n = g.size();
  vector<int> indeg(n);
  for (int v = 0; v < n; ++v) {
    for (auto u: g[v]) ++indeg[u];
  }
  deque<int> q;
  for (int v = 0; v < n; ++v) {
    if (indeg[v] == 0) {
      q.emplace_back(v);
    }
  }
  vector<int> res;
  while (not q.empty()) {
    auto v = q.front();
    q.pop_front();
    res.push_back(v);
    for (auto u: g[v]) {
      if (--indeg[u] == 0) {
        q.push_back(u);
      }
    }
  }
  if (int(res.size()) != n) {
    return nullopt;  // not a DAG
  }
  return res;
}

auto solve() -> bool {
  int H = in, W = in;
  auto A = vector(H, vector(W, 0LL));
  REP(i, H) REP(j, W) A[i][j] = Int(in);
  for (int i = H - 1; i >= 0; --i) {
    bool all_zero = [&]() {
      REP(j, W) if (A[i][j] != 0) return false;
      return true;
    }();
    if (all_zero) {
      swap(A[i], A.back());
      A.pop_back();
      --H;
    }
  }
  if (A.empty()) return true;
  for (int j = W - 1; j >= 0; --j) {
    bool all_zero = [&]() {
      REP(i, H) if (A[i][j] != 0) return false;
      return true;
    }();
    if (all_zero) {
      REP(i, H) {
        swap(A[i][j], A[i].back());
        A[i].pop_back();
      }
      --W;
    }
  }
  if (W == 0) return true;

  vector<Int> rsum(H), rc(H);
  REP(i, H) {
    REP(j, W) {
      rsum[i] += A[i][j];
      if (A[i][j] > 0) ++rc[i];
    }
  }

  {
    vector<int> idx(A.size());
    std::iota(idx.begin(), idx.end(), 0);
    sort(ALL(idx), [&](int i, int j) {
      return rsum[i] * rc[j] < rsum[j] * rc[i];
    });
    Int maxval = -1;
    for (int i: idx) {
      Int mi = kBig<Int>, ma = -1;
      REP(j, W) {
        if (A[i][j] == 0) continue;
        chmin(mi, A[i][j]);
        chmax(ma, A[i][j]);
      }
      if (mi < maxval) {
        return false;
      }
      chmax(maxval, ma);
    }
  }

  vector<Compressed<Int>> co;
  int V = 0;
  REP(i, H) {
    co.emplace_back(A[i]);
    V += co[i].size() + 1;
  }
  vector<vector<int>> g(W + V + 1);
  int B = W;
  REP(i, H) {
    REP(j, co[i].size()) {
      if (co[i].value(j) == 0) continue;
      g[B + j].push_back(B + j + 1);
    }
    REP(j, W) {
      if (A[i][j] == 0) continue;
      int v = co[i].index(A[i][j]);
      g[B + v].push_back(j);
      g[j].push_back(B + v + 1);
    }
    B += co[i].size() + 1;
  }
  auto res = topological_sort(g);
  return res.has_value();
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
